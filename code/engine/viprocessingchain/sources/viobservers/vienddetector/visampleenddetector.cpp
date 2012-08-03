#include "visampleenddetector.h"

ViSampleEndDetector::ViSampleEndDetector()
	: ViEndDetector()
{
	setThreshold(ViEndDetector::SongStart, 500, 0.05);
	setThreshold(ViEndDetector::SongEnd, 4000, 0.01);
	setThreshold(ViEndDetector::RecordStart, 500, 0.05);
	setThreshold(ViEndDetector::RecordEnd, 10000, 0.01);
}

void ViSampleEndDetector::setThreshold(ViEndDetector::Type type, int milliseconds, qreal value)
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

void ViSampleEndDetector::initialize()
{
	ViEndDetector::initialize();

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

void ViSampleEndDetector::run()
{
	qreal value;

	for(int i = 0; i < mData->size(); ++i)
	{
		value = qAbs(mData->at(i));

		if(!mSongRunning)
		{
			mTotalRecordValue += value - mRecordCache.dequeue();
			mRecordCache.enqueue(value);
			if(!mRecordRunning && mTotalRecordValue > mRecordStartValueThreshold && mSampleCounter >= mRecordStartSampleThreshold)
			{
				ViAudioPosition position(mSampleCounter + i, ViAudioPosition::Samples, mFormat);
				setRecordStart(position);
				setRecordEndCache();
			}
			else if(mRecordRunning && mTotalRecordValue < mRecordEndValueThreshold && mSampleCounter >= mRecordEndSampleThreshold)
			{
				ViAudioPosition position(mSampleCounter + i, ViAudioPosition::Samples, mFormat);
				setRecordEnd(position);
				setRecordStartCache();
			}
		}
		
		if(mRecordRunning)
		{
/*if(i==0)
cout<<"****************: "<<mSongRunning<<" ("<<mTotalSongValue<<", "<<mSongEndValueThreshold<<") ("<<mSampleCounter<<", "<<mSongEndSampleThreshold<<")"<<endl;*/
			mTotalSongValue += value - mSongCache.dequeue();
			mSongCache.enqueue(value);
			if(!mSongRunning && mTotalSongValue > mSongStartValueThreshold && mSampleCounter >= mSongStartSampleThreshold)
			{
				ViAudioPosition position(mSampleCounter + i, ViAudioPosition::Samples, mFormat);
				setSongStart(position);
				setSongEndCache();
			}
			else if(mSongRunning && mTotalSongValue < mSongEndValueThreshold && mSampleCounter >= mSongEndSampleThreshold)
			{
				ViAudioPosition position(mSampleCounter + i, ViAudioPosition::Samples, mFormat);
				setSongEnd(position);
				setSongStartCache();
				setRecordEndCache();
			}
		}
	}

	mSampleCounter += mData->size();
}

void ViSampleEndDetector::setSongStartCache()
{
	mSongCache.clear();
	for(int i = 0; i < mSongStartSampleThreshold; ++i)
	{
		mSongCache.enqueue(0);
	}
	mTotalSongValue = 0;
}

void ViSampleEndDetector::setSongEndCache()
{
	mSongCache.clear();
	for(int i = 0; i < mSongEndSampleThreshold; ++i)
	{
		mSongCache.enqueue(1);
	}
	mTotalSongValue = mSongEndSampleThreshold;
}

void ViSampleEndDetector::setRecordStartCache()
{
	mRecordCache.clear();
	for(int i = 0; i < mRecordStartSampleThreshold; ++i)
	{
		mRecordCache.enqueue(0);
	}
	mTotalRecordValue = 0;
}

void ViSampleEndDetector::setRecordEndCache()
{
	mRecordCache.clear();
	for(int i = 0; i < mRecordEndSampleThreshold; ++i)
	{
		mRecordCache.enqueue(1);
	}
	mTotalRecordValue = mRecordEndSampleThreshold;
}
