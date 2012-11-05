#include "viprojecthandler.h"
#include "viaudiocodec.h"

#define MINIMUM_SONG_LENGTH 20

ViProjectHandler::ViProjectHandler(ViProcessingChain *chain)
	: ViHandler(chain)
{
	mProject = NULL;
	mSongDetector = NULL;
	mObject = ViAudioObject::createNull();
	QObject::connect(this, SIGNAL(statusChanged(QString)), mChain, SIGNAL(statusChanged(QString)));
	QObject::connect(this, SIGNAL(progressStarted()), mChain, SIGNAL(progressStarted()));
	QObject::connect(this, SIGNAL(progressFinished()), mChain, SIGNAL(progressFinished()));
	QObject::connect(mChain, SIGNAL(attached(ViProcessor*)), this, SLOT(setDetector(ViProcessor*)));
}

ViProjectHandler::~ViProjectHandler()
{
	clearProject();
}

void ViProjectHandler::clearProject()
{
	if(mProject != NULL)
	{
		delete mProject;
		mProject = NULL;
	}
}

void ViProjectHandler::create(QString name, QString filePath, short recordSides)
{
	if(mProject != NULL)
	{
		QObject::disconnect(&mChain->mAudioObjects, SIGNAL(finished(ViAudioObjectPointer)), this, SLOT(addAudioObject(ViAudioObjectPointer)));
	}
	clearProject();
	mProject = new ViProject(filePath, name);
	mProject->setSideCount(recordSides);
	mProject->nextSide();
	mProject->save();

	QObject::connect(mChain, SIGNAL(recordStarted()), this, SLOT(startRecord()));
	QObject::connect(mChain, SIGNAL(recordEnded()), this, SLOT(endRecord()));
	QObject::connect(mChain, SIGNAL(songStarted()), this, SLOT(startSong()));
	QObject::connect(mChain, SIGNAL(songEnded()), this, SLOT(endSong()));

	QObject::connect(&mChain->mAudioObjects, SIGNAL(finished(ViAudioObjectPointer)), this, SLOT(addAudioObject(ViAudioObjectPointer)));
	emit progressStarted();
	changeStatus("Please start the record");
}

void ViProjectHandler::setDetector(ViProcessor *processor)
{
	ViSongDetector *detector = dynamic_cast<ViSongDetector*>(processor);
	if(detector != NULL)
	{
		mSongDetector = detector;
	}
}

void ViProjectHandler::addAudioObject(ViAudioObjectPointer object)
{
	mObject = object;
	ViBuffer *buffer = object->correctedBuffer();
	qreal songLength = ViAudioPosition::convertPosition(buffer->size(), ViAudioPosition::Samples, ViAudioPosition::Seconds, buffer->format());
	if(songLength > MINIMUM_SONG_LENGTH)
	{
		QObject::connect(mChain->mFileOutput, SIGNAL(finished()), this, SLOT(finishWriting()));
		if(mSongDetector != NULL)
		{
			mChain->mFileOutput->setSongInfo(mSongDetector->songInfo());
		}
		mChain->mFileOutput->setFormat(buffer->format());
		mChain->mFileOutput->setBuffer(buffer);
		mChain->mFileOutput->setFile(mProject->originalPath(), mProject->nextOriginalSongNumber(), mChain->mFileOutput->format().codec()->extensions()[0]);
		mChain->mFileOutput->start();
	}
}

void ViProjectHandler::finishWriting()
{
	mObject = ViAudioObject::createNull(); // Ensures that we have a reference to the object until the buffer was written.
	ViFileSongInfo info = ViFileSongInfo(mChain->mFileOutput->songInfo());
	info.setOriginalFilePath(mChain->mFileOutput->filePath());
	mProject->addSong(info);
	mProject->save();
	QObject::disconnect(mChain->mFileOutput, SIGNAL(finished()), this, SLOT(finishWriting()));
}

void ViProjectHandler::changeStatus(QString status)
{
	emit statusChanged(status);
}

void ViProjectHandler::startRecord()
{
	changeStatus("Waiting for song to start");
}

void ViProjectHandler::endRecord()
{
	if(mProject->nextSide())
	{
		if(mProject->currentSide() % 2 == 0)
		{
			changeStatus("Please turn over the record");
		}
		else
		{
			changeStatus("Please start the next record");
		}
	}
	else
	{
		QObject::disconnect(mChain, SIGNAL(recordStarted()), this, SLOT(startRecord()));
		QObject::disconnect(mChain, SIGNAL(recordEnded()), this, SLOT(endRecord()));
		QObject::disconnect(mChain, SIGNAL(songStarted()), this, SLOT(startSong()));
		QObject::disconnect(mChain, SIGNAL(songEnded()), this, SLOT(endSong()));
		emit progressFinished();
	}
}

void ViProjectHandler::startSong()
{
	changeStatus("Processing song");
}

void ViProjectHandler::endSong()
{
	changeStatus("Waiting for song to start");
}
