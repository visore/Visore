#include "vienddetector.h"

ViEndDetector::ViEndDetector()
	: ViObserver()
{
	setThreshold(3000, 0.02);
	setMinimumLength(90000);
}

void ViEndDetector::setThreshold(int milliseconds, qreal value)
{
	mMillisecondThreshold = milliseconds;
	mValue = value;
}

void ViEndDetector::setMinimumLength(int milliseconds)
{
	mMinimumMilliseconds = milliseconds;
}

void ViEndDetector::initialize()
{
	mSamplesThreshold = ViAudioPosition::convertPosition(mMillisecondThreshold, ViAudioPosition::Milliseconds, ViAudioPosition::Samples, mFormat);
	mMinimumSamples = ViAudioPosition::convertPosition(mMinimumMilliseconds, ViAudioPosition::Milliseconds, ViAudioPosition::Samples, mFormat);
	mValueThreshold = mSamplesThreshold * mValue; //Same as taking the average after each iteration. just saves some computational time.
	clear();
	for(int i = 0; i < mSamplesThreshold; ++i)
	{
		mCache.enqueue(0);
	}
	mDetected = false;
}

void ViEndDetector::run()
{
	qreal value;
	qreal old = mTotalValue;
	for(int i = 0; i < mData->size(); ++i)
	{
for(int h = 0; h < 10000; h++) int t = 100*3.25/789+94145-85.20*9654/85;
		value = qAbs(mData->at(i));
		mTotalValue += value - mCache.dequeue();
		mCache.enqueue(value);
		if(mTotalValue < mValueThreshold 
			&& (mSampleCounter + i) > mMinimumSamples
			&& old > mValueThreshold
			&& !mDetected)
		{
			mDetected = true;
			mSampleCounter += i + 1;
			LOG(QString("Detected end of song at second " + QString::number(ViAudioPosition::convertPosition(mSampleCounter, ViAudioPosition::Samples, ViAudioPosition::Seconds, mFormat), 'f', 2) + "."));
			emit endDetected(ViAudioPosition(mSampleCounter, ViAudioPosition::Samples, mFormat));
			return;
		}
	}
	mSampleCounter += mData->size();
}

void ViEndDetector::clear()
{
	mCache.clear();
	mTotalValue = 0;
	mSampleCounter = 0;
}
