#include "viwavecollection.h"

ViWaveCollection::ViWaveCollection()
{
	for(int i = 0; i < MAXIMUM_COMPRESSION_LEVELS; ++i)
	{
		mWaves.append(new ViWave());
		mSampleLevels.append(FIRST_COMPRESSION_LEVEL * (i + 1));
	}
	reset();
}

ViWaveCollection::~ViWaveCollection()
{
	for(int i = 0; i < MAXIMUM_COMPRESSION_LEVELS; ++i)
	{
		delete mWaves[i];
		mWaves[i] = NULL;
	}
}

void ViWaveCollection::append(double value)
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
	if(mTotalCounter == FIRST_COMPRESSION_LEVEL)
	{
		if(mMaximumCounter > 0)
		{
			mAverageMaximum /= mMaximumCounter;
		}
		if(mMinimumCounter > 0)
		{
			mAverageMinimum /= mMinimumCounter;
		}
		mWaves[0]->append(new ViAmplitude(mMaximum, mMinimum, mAverageMaximum, mAverageMinimum));
		reset();
	}
	update();
}

int ViWaveCollection::size(int level)
{
	return mWaves[level]->size();
}

int ViWaveCollection::samples(int level)
{
	return mSampleLevels[level];
}

double ViWaveCollection::maximum(int level, int position)
{
	return mWaves[level]->at(position)->maximum();
}

double ViWaveCollection::minimum(int level, int position)
{
	return mWaves[level]->at(position)->minimum();
}

double ViWaveCollection::maximumAverage(int level, int position)
{
	return mWaves[level]->at(position)->maximumAverage();
}

double ViWaveCollection::minimumAverage(int level, int position)
{
	return mWaves[level]->at(position)->minimumAverage();
}

void ViWaveCollection::update()
{
	for(int i = 0; i < MAXIMUM_COMPRESSION_LEVELS - 1; ++i)
	{
		int start = mWaves[i+1]->size() * 2;
		if(start < 1)
		{
			start = 1;
		}
		for(int j = start; j < mWaves[i]->size(); j += 2)
		{
			double maximum = mWaves[i]->at(j)->maximum();
			if(mWaves[i]->at(j-1)->maximum() > maximum)
			{
				maximum = mWaves[i]->at(j-1)->maximum();
			}

			double minimum = mWaves[i]->at(j)->minimum();
			if(mWaves[i]->at(j-1)->minimum() < minimum)
			{
				minimum = mWaves[i]->at(j-1)->minimum();
			}

			double averageMaximum = (mWaves[i]->at(j)->maximumAverage() + mWaves[i]->at(j-1)->maximumAverage()) / 2.0;
			double averageMinimum = (mWaves[i]->at(j)->minimumAverage() + mWaves[i]->at(j-1)->minimumAverage()) / 2.0;
			mWaves[i+1]->append(new ViAmplitude(maximum, minimum, averageMaximum, averageMinimum));
		}
	}
}

void ViWaveCollection::reset()
{
	mMaximum = -1;
	mMinimum = 1;
	mAverageMaximum = 0;
	mAverageMinimum = 0;
	mMaximumCounter = 0;
	mMinimumCounter = 0;
	mTotalCounter = 0;
}
