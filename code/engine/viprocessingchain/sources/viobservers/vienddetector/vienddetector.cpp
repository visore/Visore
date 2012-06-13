#include "vienddetector.h"
#include "viaudioposition.h"

ViEndDetector::ViEndDetector()
	: ViObserver()
{
	setThreshold(2000, 0.01);
}

void ViEndDetector::setThreshold(int milliseconds, qreal value)
{
	mMillisecondThreshold = milliseconds;
	mValueThreshold = value;
}

void ViEndDetector::initialize()
{
	mSamplesThreshold = ViAudioPosition::convertPosition(mMillisecondThreshold, ViAudioPosition::Milliseconds, ViAudioPosition::Samples, mFormat);
	clear();
}

void ViEndDetector::run()
{
	int value;
	for(int i = 0; i < mData->size(); ++i)
	{
		mTotalValue -= mCache.dequeue();
		value = mData->at(i);
		mTotalValue += value;
		mCache.enqueue(value);
		if(mTotalValue / mSamplesThreshold <= mValueThreshold)
		{
			emit endDetected();
			clear();
			break;
		}
	}
}

void ViEndDetector::clear()
{
	mCache.clear();
	for(int i = 0; i < mSamplesThreshold; ++i)
	{
		mCache.enqueue(1);
	}
	mConfidence = 0;
	mTotalValue = mSamplesThreshold;
}
