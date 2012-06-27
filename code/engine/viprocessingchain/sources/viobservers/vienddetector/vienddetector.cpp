#include "vienddetector.h"

ViEndDetector::ViEndDetector()
	: ViObserver()
{
	setThreshold(ViEndDetector::SongStart, 500, 0.05);
	setThreshold(ViEndDetector::SongEnd, 4000, 0.01);
	setThreshold(ViEndDetector::RecordStart, 500, 0.05);
	setThreshold(ViEndDetector::RecordEnd, 10000, 0.01);
	mSongStarted = false;
	mRecordStarted = false;
}

void ViEndDetector::setThreshold(ViEndDetector::Type type, int milliseconds, qreal value)
{
	if(type == ViEndDetector::SongStart)
	{
		mSongStartTime = milliseconds;
		mSongStartValue = value;
	}
	else if(type == ViEndDetector::SongEnd)
	{
		mSongEndTime = milliseconds;
		mSongEndValue = value;
	}
	else if(type == ViEndDetector::RecordStart)
	{
		mRecordStartTime = milliseconds;
		mRecordStartValue = value;
	}
	else if(type == ViEndDetector::RecordEnd)
	{
		mRecordEndTime = milliseconds;
		mRecordEndValue = value;
	}
}

void ViEndDetector::initialize()
{
	mSampleCounter = 0;

	mSongStartSampleThreshold = ViAudioPosition::convertPosition(mSongStartTime, ViAudioPosition::Milliseconds, ViAudioPosition::Samples, mFormat);
	mSongEndSampleThreshold = ViAudioPosition::convertPosition(mSongEndTime, ViAudioPosition::Milliseconds, ViAudioPosition::Samples, mFormat);
	mRecordStartSampleThreshold = ViAudioPosition::convertPosition(mRecordStartTime, ViAudioPosition::Milliseconds, ViAudioPosition::Samples, mFormat);
	mRecordEndSampleThreshold = ViAudioPosition::convertPosition(mRecordEndTime, ViAudioPosition::Milliseconds, ViAudioPosition::Samples, mFormat);

	//Same as taking the average after each iteration. Just saves some computational time.
	mSongStartValueThreshold = mSongStartValue * mSongStartSampleThreshold;
	mSongEndValueThreshold = mSongEndValue * mSongEndSampleThreshold;
	mRecordStartValueThreshold = mRecordStartValue * mRecordStartSampleThreshold;
	mRecordEndValueThreshold = mRecordEndValue * mRecordEndSampleThreshold;

	setSongStartCache();
	setRecordStartCache();
}

void ViEndDetector::run()
{
	qreal value;

	for(int i = 0; i < mData->size(); ++i)
	{
		value = qAbs(mData->at(i));

		if(!mSongStarted)
		{
			mTotalRecordValue += value - mRecordCache.dequeue();
			mRecordCache.enqueue(value);
			if(!mRecordStarted && mTotalRecordValue > mRecordStartValueThreshold && mSampleCounter >= mRecordStartSampleThreshold)
			{
				ViAudioPosition position(mSampleCounter + i, ViAudioPosition::Samples, mFormat);
				LOG(QString("Record started: " + QString::number(position.position(ViAudioPosition::Seconds), 'f', 2) + " sec."));
				emit recordStarted(position);

				mRecordStarted = true;
				setRecordEndCache();
			}
			else if(mRecordStarted && mTotalRecordValue < mRecordEndValueThreshold && mSampleCounter >= mRecordEndSampleThreshold)
			{
				ViAudioPosition position(mSampleCounter + i, ViAudioPosition::Samples, mFormat);
				LOG(QString("Record ended: " + QString::number(position.position(ViAudioPosition::Seconds), 'f', 2) + " sec."));
				emit recordEnded(position);

				mRecordStarted = false;
				setRecordStartCache();
			}
		}
		
		if(mRecordStarted)
		{
			mTotalSongValue += value - mSongCache.dequeue();
			mSongCache.enqueue(value);
			if(!mSongStarted && mTotalSongValue > mSongStartValueThreshold && mSampleCounter >= mSongStartSampleThreshold)
			{
				ViAudioPosition position(mSampleCounter + i, ViAudioPosition::Samples, mFormat);
				LOG(QString("Song started: " + QString::number(position.position(ViAudioPosition::Seconds), 'f', 2) + " sec."));
				emit songStarted(position);

				mSongStarted = true;
				setSongEndCache();
			}
			else if(mSongStarted && mTotalSongValue < mSongEndValueThreshold && mSampleCounter >= mSongEndSampleThreshold)
			{
				ViAudioPosition position(mSampleCounter + i, ViAudioPosition::Samples, mFormat);
				LOG(QString("Song ended: " + QString::number(position.position(ViAudioPosition::Seconds), 'f', 2) + " sec."));
				emit songEnded(position);

				mSongStarted = false;
				setSongStartCache();
				setRecordEndCache();
			}
		}
	}

	mSampleCounter += mData->size();
}

void ViEndDetector::setSongStartCache()
{
	mSongCache.clear();
	for(int i = 0; i < mSongStartSampleThreshold; ++i)
	{
		mSongCache.enqueue(0);
	}
	mTotalSongValue = 0;
}

void ViEndDetector::setSongEndCache()
{
	mSongCache.clear();
	for(int i = 0; i < mSongEndSampleThreshold; ++i)
	{
		mSongCache.enqueue(1);
	}
	mTotalSongValue = mSongEndSampleThreshold;
}

void ViEndDetector::setRecordStartCache()
{
	mRecordCache.clear();
	for(int i = 0; i < mRecordStartSampleThreshold; ++i)
	{
		mRecordCache.enqueue(0);
	}
	mTotalRecordValue = 0;
}

void ViEndDetector::setRecordEndCache()
{
	mRecordCache.clear();
	for(int i = 0; i < mRecordEndSampleThreshold; ++i)
	{
		mRecordCache.enqueue(1);
	}
	mTotalRecordValue = mRecordEndSampleThreshold;
}
