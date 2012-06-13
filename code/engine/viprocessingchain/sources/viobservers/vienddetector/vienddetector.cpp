#include "vienddetector.h"

ViEndDetector::ViEndDetector()
	: ViObserver()
{
	setThreshold(3000, 0.02);
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
	for(int i = 0; i < mSamplesThreshold; ++i)
	{
		mCache.enqueue(1);
	}
	mDetected = false;
}

void ViEndDetector::run()
{
	qreal value;
	for(int i = 0; i < mData->size(); ++i)
	{
		value = qAbs(mData->at(i));
		mTotalValue += value - mCache.dequeue();
		mCache.enqueue(value);
		if((mTotalValue / mSamplesThreshold) <= mValueThreshold && !mDetected)
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
	mTotalValue = mSamplesThreshold;
	mSampleCounter = 0;
}
