#include "visongdetector.h"
#include "viaudioposition.h"

//Interval (milliseconds) use to request info
#define REQUEST_SAMPLES_1 10000
#define REQUEST_SAMPLES_2 30000
#define REQUEST_SAMPLES_3 45000
#define REQUEST_SAMPLES_4 90000
#define REQUEST_SAMPLES_5 120000

ViSongDetector::ViSongDetector()
	: ViObserver()
{
	disable();
	mIdentifier = NULL;
	mRequestsSent = 0;
}

ViSongDetector::~ViSongDetector()
{
	if(mIdentifier != NULL)
	{
		QObject::disconnect(mIdentifier, SIGNAL(songDetected(ViSongInfo)), this, SLOT(setSong(ViSongInfo)));
	}
}

void ViSongDetector::setSong(ViSongInfo info)
{
	mSongInfo = info;
	emit songDetected(mSongInfo);
}

void ViSongDetector::initialize()
{
	mRequestsSent = 0;
	mSongInfo = ViSongInfo();
}

void ViSongDetector::finalize()
{
}

void ViSongDetector::execute()
{
	if(!mObject->isSong() || mIdentifier == NULL)
	{
		return;
	}
	mIsEnabledMutex.lock();
	bool isEnabled = mIsEnabled;
	mIsEnabledMutex.unlock();
	if(isEnabled && !mIdentifier->found())
	{
		qint64 bufferLength = ViAudioPosition::convertPosition(mObject->outputBuffer()->size(), ViAudioPosition::Bytes, ViAudioPosition::Milliseconds, mObject->outputBuffer()->format());
		if(	(mRequestsSent == 0 && bufferLength >= REQUEST_SAMPLES_1) ||
			(mRequestsSent == 1 && bufferLength >= REQUEST_SAMPLES_2) ||
			(mRequestsSent == 2 && bufferLength >= REQUEST_SAMPLES_3) ||
			(mRequestsSent == 3 && bufferLength >= REQUEST_SAMPLES_4) ||
			(mRequestsSent == 4 && bufferLength >= REQUEST_SAMPLES_5))
		{
			if(bufferLength >= REQUEST_SAMPLES_5)
			{
				mRequestsSent = 5;
			}
			else if(bufferLength >= REQUEST_SAMPLES_4)
			{
				mRequestsSent = 4;
			}
			else if(bufferLength >= REQUEST_SAMPLES_3)
			{
				mRequestsSent = 3;
			}
			else if(bufferLength >= REQUEST_SAMPLES_2)
			{
				mRequestsSent = 2;
			}
			else if(bufferLength >= REQUEST_SAMPLES_1)
			{
				mRequestsSent = 1;
			}
			
			if(mIdentifier != NULL)
			{
				mIdentifier->identify(mObject->outputBuffer());
			}
		}
	}
}

void ViSongDetector::setIdentifier(ViSongIdentifier *identifier)
{
	if(mIdentifier != NULL)
	{
		QObject::disconnect(mIdentifier, SIGNAL(songDetected(ViSongInfo)), this, SLOT(setSong(ViSongInfo)));
	}
	mIdentifier = identifier;
	QObject::connect(mIdentifier, SIGNAL(songDetected(ViSongInfo)), this, SLOT(setSong(ViSongInfo)));
}

ViSongInfo& ViSongDetector::songInfo()
{
	return mSongInfo;
}
