#include <viaudiorecorder.h>

#define IDLE_SIZE 5242880 // 5MB
#define IDLE_TIME 1000 // 1 second
#define LENGTH_CUTOFF 15 // 15 seconds

ViAudioRecorder::ViAudioRecorder()
{
	mProject = NULL;
	mType = ViAudio::Undefined;

	mCurrentTrack = 0;
	mCurrentSide = 0;
	mSides = 0;
    mExistingProject = false;
	mWaitForQueue = false;

	mIdleTimer.setInterval(IDLE_TIME);
	QObject::connect(&mIdleTimer, SIGNAL(timeout()), this, SLOT(checkSize()));

	mSegmentDetector.setMultiShot();
	QObject::connect(&mSegmentDetector, SIGNAL(songStarted(ViAudioPosition)), this, SLOT(startSong()));
	QObject::connect(&mSegmentDetector, SIGNAL(songEnded(ViAudioPosition)), this, SLOT(endSong()));
	QObject::connect(&mSegmentDetector, SIGNAL(recordStarted(ViAudioPosition)), this, SLOT(startRecord()));
	QObject::connect(&mSegmentDetector, SIGNAL(recordEnded(ViAudioPosition)), this, SLOT(endRecord()));
}

ViAudioRecorder::~ViAudioRecorder()
{
	mProject = NULL;
}

bool ViAudioRecorder::record(ViProject *project, ViAudio::Type type, ViAudioFormat format, int sides, bool detectMetadata)
{
	emit started();
	setProgress(-1); //Infinite
	mQueue.clear();
    mCurrentTrack = 0;
    mCurrentSide = 0;
	mProject = project;
	mType = type;
	mInput.setFormat(format);

	mDetectMetadata = detectMetadata;
	mWaitForQueue = false;

    if(mProject->objectCount() > 0)
    {
        mExistingProject = true;
        mSides = mProject->sideCount();
    }
    else
    {
        mExistingProject = false;
        mSides = sides;
    }

	if(mType == ViAudio::Target || mType == ViAudio::Corrupted)
	{
		nextObject();
		emit statused("Waiting for record to start");
		return true;
	}
	LOG("Invalid type specified for the recording. The type must be Target or Corrupted.", QtCriticalMsg);
	setProgress(100);
	emit finished();
	return false;
}

void ViAudioRecorder::nextObject(bool startTimer)
{
	mIdleTimer.stop();
	mSegmentDetector.stop();

	mObject = ViAudioObject::create();
    mObject->setSideNumber(mCurrentSide);
    mObject->setTrackNumber(mCurrentTrack);

	mInput.setBuffer(mObject->buffer(mType));
	mSegmentDetector.process(mObject, mType); // Important: The preious statment will create a buffer and set the format. Must be done before this is executed
	mInput.start();

	if(startTimer) mIdleTimer.start();
}

void ViAudioRecorder::finish()
{
	QObject::disconnect(mProject, SIGNAL(finished()), this, SLOT(finish()));
	mObject.setNull();
	mQueue.clear();
	setProgress(100);
	emit finished();
}

void ViAudioRecorder::finishProject()
{
	emit statused("Waiting for project to finish");
	if(!mQueue.isEmpty())
	{
		mWaitForQueue = true;
	}
	else if((mProject == NULL || mProject->isFinished())) // If project is not finished, wait for it to finish
	{
		finish();
	}
	else
	{
		QObject::connect(mProject, SIGNAL(finished()), this, SLOT(finish()), Qt::UniqueConnection);
	}
}

void ViAudioRecorder::startSong()
{
	++mCurrentTrack;
	nextObject(false);
	emit statused("Processing track");
}

void ViAudioRecorder::endSong()
{
	mSegmentDetector.stop();
	if(mObject->length(ViAudioPosition::Seconds) > LENGTH_CUTOFF)
	{
		mQueue.enqueue(mObject);
		ViAudioObjectPointer object = mProject->object(mObject->sideNumber(), mObject->trackNumber());
		bool detect = true;
		if(!object.isNull() && object->hasMetadata()  && object->hasCover())
		{
			detect = false;
		}
		if(mDetectMetadata && detect)
		{
			QObject::connect(mObject.data(), SIGNAL(metadataDetected(bool)), this, SLOT(serialize()));
			mObject->detectMetadata();
		}
		else
		{
			serialize();
		}
	}
	else
	{
		--mCurrentTrack;
	}
	nextObject();
	emit statused("Waiting for track to start");
}

void ViAudioRecorder::startRecord()
{
	++mCurrentSide;
	nextObject();
	emit statused("Waiting for track to start");
}

void ViAudioRecorder::endRecord()
{
	mIdleTimer.stop();
	mInput.stop();

	if(mCurrentTrack == 0) // If there wasn't a song start yet. Eg: record takes very long to start
	{
		--mCurrentSide;
		nextObject();
	}
	else
	{
		mCurrentTrack = 0;
		if(mCurrentSide == mSides)
		{
			mSegmentDetector.stop();
			finishProject();
		}
		else
		{
			nextObject();
			if(mCurrentSide % 2 == 0)
			{
				emit statused("Please mount the next record");
			}
			else
			{
				emit statused("Please turn over the record");
			}
		}
	}
}

void ViAudioRecorder::serialize()
{
    ViAudioObjectPointer object = mQueue.dequeue();
	QObject::disconnect(object.data(), SIGNAL(metadataDetected(bool)), this, SLOT(serialize()));
	if(mProject != NULL)
	{
		if(mExistingProject)
		{
			ViAudioObjectPointer existingObject = mProject->object(object->sideNumber(), object->trackNumber());
			existingObject->transferBuffer(object, mType);
			if(!existingObject->hasMetadata() && object->hasMetadata())
			{
				existingObject->setMetadata(object->metadata());
			}
			if(!existingObject->hasCover() && object->hasCover())
			{
				existingObject->metadata().setCover(object->metadata().cover());
			}
			object = existingObject;
        }
        else
        {
            mProject->add(object);
        }
		object->encode(mType, true);
	}
	if(mWaitForQueue)
	{
		finishProject();
	}
}

void ViAudioRecorder::checkSize()
{
    if(mObject->buffer(mType)->size() >= IDLE_SIZE)
	{
		nextObject();
	}
}
