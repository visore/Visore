#include "vienddetector.h"

ViEndDetector::ViEndDetector()
	: ViObserver()
{
	mSongRunning = false;
	mRecordRunning = false;
}

void ViEndDetector::initialize()
{
	/*mSongRunning = false;
	mRecordRunning = false;
cout<<"*****"<<endl;*/
}

void ViEndDetector::setRecordStart(ViAudioPosition position)
{
	if(!mRecordRunning)
	{
		mRecordRunning = true;
		LOG(QString("Record started: " + QString::number(position.position(ViAudioPosition::Seconds), 'f', 2) + " sec."));
		emit recordStarted(position);
	}
}

void ViEndDetector::setSongStart(ViAudioPosition position)
{
	if(!mSongRunning)
	{
		mSongRunning = true;
		if(!mRecordRunning)
		{
			setRecordStart(position);
		}
		LOG(QString("Song started: " + QString::number(position.position(ViAudioPosition::Seconds), 'f', 2) + " sec."));
		emit songStarted(position);
	}
}

void ViEndDetector::setRecordEnd(ViAudioPosition position)
{
	if(mRecordRunning)
	{
		mRecordRunning = false;
		if(mSongRunning)
		{
			setSongEnd(position);
		}
		LOG(QString("Record ended: " + QString::number(position.position(ViAudioPosition::Seconds), 'f', 2) + " sec."));
		emit recordEnded(position);
	}
}

void ViEndDetector::setSongEnd(ViAudioPosition position)
{
	if(mSongRunning)
	{
		mSongRunning = false;
		LOG(QString("Song ended: " + QString::number(position.position(ViAudioPosition::Seconds), 'f', 2) + " sec."));
		emit songEnded(position);
	}
}
