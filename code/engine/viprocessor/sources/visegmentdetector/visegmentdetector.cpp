#include <visegmentdetector.h>

ViSegmentDetector::ViSegmentDetector(ViProcessor::ChannelMode mode)
    : ViProcessor(mode)
{
	mSongRunning = false;
	mRecordRunning = false;
}

ViSegmentDetector::~ViSegmentDetector()
{
}

void ViSegmentDetector::setRecordStart(ViAudioPosition position)
{
	if(!mRecordRunning)
	{
		mRecordRunning = true;
		LOG(QString("Record started: " + QString::number(position.position(ViAudioPosition::Seconds), 'f', 2) + " sec."));
		emit recordStarted(position);
	}
}

void ViSegmentDetector::setSongStart(ViAudioPosition position)
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

void ViSegmentDetector::setRecordEnd(ViAudioPosition position)
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

void ViSegmentDetector::setSongEnd(ViAudioPosition position)
{
	if(mSongRunning)
	{
		mSongRunning = false;
		LOG(QString("Song ended: " + QString::number(position.position(ViAudioPosition::Seconds), 'f', 2) + " sec."));
		emit songEnded(position);
	}
}
