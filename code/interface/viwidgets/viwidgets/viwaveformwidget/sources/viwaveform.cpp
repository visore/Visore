#include "viwaveform.h"

ViWaveForm::ViWaveForm()
{
	mCompression = 0;
	mBufferSize = 0;
	reset();
}

void ViWaveForm::append(qreal value)
{
	++mTotalCounter;
	if(value > mMaximum)
	{
		mMaximum = value;
	}
	else if(value < mMinimum)
	{
		mMinimum = value;
	}
	if(value > 0)
	{
		++mMaximumCounter;
		mAverageMaximum += value;
	}
	else if(value < 0)
	{
		++mMinimumCounter;
		mAverageMinimum += value;
	}
	if(mTotalCounter == mCompression)
	{
		if(mMaximumCounter > 0)
		{
			mAverageMaximum /= mMaximumCounter;
		}
		if(mMinimumCounter > 0)
		{
			mAverageMinimum /= mMinimumCounter;
		}
		if(size() >= mBufferSize)
		{
			mWave.removeFirst();
		}
		mWave.append(new ViAmplitude(mMaximum, mMinimum, mAverageMaximum, mAverageMinimum));
		reset();
	}
}

qint32 ViWaveForm::size()
{
	return mWave.size();
}

qreal ViWaveForm::maximum(qint32 position)
{
	return mWave[position]->maximum();
}

qreal ViWaveForm::minimum(qint32 position)
{
	return mWave[position]->minimum();
}

qreal ViWaveForm::maximumAverage(qint32 position)
{
	return mWave[position]->maximumAverage();
}

qreal ViWaveForm::minimumAverage(qint32 position)
{
	return mWave[position]->minimumAverage();
}

void ViWaveForm::setCompression(qint32 compression)
{
	mCompression = compression;
}

void ViWaveForm::reset()
{
	mMaximum = -1;
	mMinimum = 1;
	mAverageMaximum = 0;
	mAverageMinimum = 0;
	mMaximumCounter = 0;
	mMinimumCounter = 0;
	mTotalCounter = 0;
}

qreal ViWaveForm::setBufferSize(qint32 size)
{
	mBufferSize = size;
}

void ViWaveForm::removeFirst()
{
	mWave.removeFirst();
}

