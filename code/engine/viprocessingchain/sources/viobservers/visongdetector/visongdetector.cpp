#include "visongdetector.h"
#include "viaudioposition.h"

//Interval (milliseconds) use to request info
#define REQUEST_SAMPLES_1 10000
#define REQUEST_SAMPLES_2 30000
#define REQUEST_SAMPLES_3 90000
#define REQUEST_SAMPLES_4 110000
#define REQUEST_SAMPLES_5 130000

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
		QObject::disconnect(mIdentifier, SIGNAL(songDetected(ViSongInfo)), this, SIGNAL(songDetected(ViSongInfo)));
	}
}

void ViSongDetector::initialize()
{
	mRequestsSent = 0;
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
		qint64 bufferLength = ViAudioPosition::convertPosition(mObject->originalBuffer()->size(), ViAudioPosition::Bytes, ViAudioPosition::Milliseconds, mObject->originalBuffer()->format());
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
				mIdentifier->identify(mObject->originalBuffer());
			}
		}
	}
}

void ViSongDetector::setIdentifier(ViSongIdentifier *identifier)
{
	if(mIdentifier != NULL)
	{
		QObject::disconnect(mIdentifier, SIGNAL(songDetected(ViSongInfo)), this, SIGNAL(songDetected(ViSongInfo)));
	}
	mIdentifier = identifier;
	QObject::connect(mIdentifier, SIGNAL(songDetected(ViSongInfo)), this, SIGNAL(songDetected(ViSongInfo)));
}
