#include <viaudiorecorder.h>

#define IDLE_SIZE 5242880 // 5MB
#define IDLE_TIME 1000 // 1 second
#define LENGTH_CUTOFF 15 // 15 seconds

ViAudioRecorder::ViAudioRecorder()
{
	mProject = NULL;
	mType = ViAudioObject::Undefined;

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

bool ViAudioRecorder::record(ViProject *project, ViAudioObject::Type type, bool detectInfo)
{
	return record(project, type, detectInfo, project->format());
}

bool ViAudioRecorder::record(ViProject *project, ViAudioObject::Type type, bool detectInfo, ViAudioFormat format)
{
	emit started();
	setProgress(-1);
	mProject = project;
	mType = type;
	mInput.setFormat(format);
	mQueue.clear();
	mDetectInfo = detectInfo;

	mProject->save();
	if(mType != ViAudioObject::Undefined)
	{
		nextObject();
		emit statused("Waiting for record to start");
		return true;
	}
	return false;
}

void ViAudioRecorder::nextObject(bool startTimer)
{
	mIdleTimer.stop();
	
	ViAudioObjectPointer newObject = ViAudioObject::create();
	newObject->setInputType(mType);
	
	mInput.setBuffer(newObject->inputBuffer());
	mSegmentDetector.process(newObject, mType); // Important: The preious statment will create a buffer and set the format. Must be done before this is executed
	mInput.start();
	mObject = newObject;

	if(startTimer)
	{
		mIdleTimer.start();
	}
}

void ViAudioRecorder::finish()
{
	QObject::disconnect(mProject, SIGNAL(finished()), this, SLOT(finish()));
	mObject.setNull();
	setProgress(100);
	emit finished();
}

void ViAudioRecorder::startSong()
{
	nextObject(false);
	emit statused("Processing track");
}

void ViAudioRecorder::endSong()
{
	mQueue.enqueue(mObject);
	if(mDetectInfo)
	{
		QObject::connect(mObject.data(), SIGNAL(infoed(bool)), this, SLOT(serialize()));
		mObject->detectSongInfo();
	}
	else
	{
		serialize();
	}
	mSegmentDetector.stop();
	nextObject();
	emit statused("Waiting for track to start");
}

void ViAudioRecorder::startRecord()
{
	nextObject();
	emit statused("Waiting for track to start");
}

void ViAudioRecorder::endRecord()
{
	if(mProject->isLastSide())
	{
		mIdleTimer.stop();
		mInput.stop();
		mSegmentDetector.stop();
		if((mProject == NULL || mProject->isFinished()) && mQueue.isEmpty()) // If project is not finished, wait for it to finish
		{
			finish();
		}
		else
		{
			QObject::connect(mProject, SIGNAL(finished()), this, SLOT(finish()), Qt::UniqueConnection);
		}
	}
	else
	{
		mProject->nextSide();
		nextObject();
		emit statused("Please turn over the record");
	}
}

void ViAudioRecorder::serialize()
{
	ViAudioObjectPointer object = mQueue.dequeue();
	QObject::disconnect(object.data(), SIGNAL(infoed(bool)), this, SLOT(serialize()));
	if(mProject != NULL && object->length(ViAudioPosition::Seconds) > LENGTH_CUTOFF)
	{
		mProject->serialize(object, mType);
	}
}

void ViAudioRecorder::checkSize()
{
	if(mObject->inputBuffer()->size() >= IDLE_SIZE)
	{
		nextObject();
	}
}
