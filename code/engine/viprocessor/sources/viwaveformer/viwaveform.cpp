#include "viwaveform.h"

ViWaveForm::ViWaveForm(qint16 level)
{
	mLevel = level;
	if(mLevel < MAXIMUM_ZOOM_LEVELS)
	{
		mNextLevel = new ViWaveForm(mLevel + 1);
	}
	else
	{
		mNextLevel = NULL;
	}
	reset();
	mIsUnderCutoff = FIRST_ZOOM_LEVEL * qPow(ZOOM_LEVEL_INCREASE, level) <= SUMMARY_CUTOFF;
}

ViWaveForm::~ViWaveForm()
{
	if(mNextLevel != NULL)
	{
		delete mNextLevel;
		mNextLevel = NULL;
	}
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
	mAverage += value;
	if(mTotalCounter == FIRST_ZOOM_LEVEL)
	{
		if(mMaximumCounter > 0)
		{
			mAverageMaximum /= mMaximumCounter;
		}
		if(mMinimumCounter > 0)
		{
			mAverageMinimum /= mMinimumCounter;
		}
		mAverage /= mTotalCounter;
		if(mLevel < MAXIMUM_ZOOM_LEVELS)
		{
			mNextLevel->appendValues(mMaximum, mMinimum, mAverageMaximum, mAverageMinimum, mAverage);
		}
		appendResults();
	}
}

void ViWaveForm::scaleValues(qreal *maximum, qreal *minimum, qreal *averageMaximum, qreal *averageMinimum, qreal *average)
{
	*maximum = UNSIGNED_CHAR_HALF_VALUE - (UNSIGNED_CHAR_HALF_VALUE * (*maximum));
	*minimum = UNSIGNED_CHAR_HALF_VALUE - (UNSIGNED_CHAR_HALF_VALUE * (*minimum));
	*averageMaximum = UNSIGNED_CHAR_HALF_VALUE - (UNSIGNED_CHAR_HALF_VALUE * (*averageMaximum));
	*averageMinimum = UNSIGNED_CHAR_HALF_VALUE - (UNSIGNED_CHAR_HALF_VALUE * (*averageMinimum));
	*average = UNSIGNED_CHAR_HALF_VALUE - (UNSIGNED_CHAR_HALF_VALUE * (*average));
}

void ViWaveForm::appendValues(qreal maximum, qreal minimum, qreal averageMaximum, qreal averageMinimum, qreal average)
{
	++mTotalCounter;
	if(maximum > mMaximum)
	{
		mMaximum = maximum;
	}
	if(minimum < mMinimum)
	{
		mMinimum = minimum;
	}
	if(averageMaximum > 0)
	{
		++mMaximumCounter;
		mAverageMaximum += averageMaximum;
	}
	if(averageMinimum < 0)
	{
		++mMinimumCounter;
		mAverageMinimum += averageMinimum;
	}
	mAverage += average;
	if(mTotalCounter == ZOOM_LEVEL_INCREASE)
	{
		if(mMaximumCounter > 0)
		{
			mAverageMaximum /= mMaximumCounter;
		}
		if(mMinimumCounter > 0)
		{
			mAverageMinimum /= mMinimumCounter;
		}
		mAverage /= mTotalCounter;
		if(mLevel < MAXIMUM_ZOOM_LEVELS)
		{
			mNextLevel->appendValues(mMaximum, mMinimum, mAverageMaximum, mAverageMinimum, mAverage);
		}
		appendResults();
	}
}

void ViWaveForm::appendResults()
{
	scaleValues(&mMaximum, &mMinimum, &mAverageMaximum, &mAverageMinimum, &mAverage);
	mMutex.lock();
	if(mIsUnderCutoff)
	{
		if((UNSIGNED_CHAR_HALF_VALUE - mMinimum) * -1 > UNSIGNED_CHAR_HALF_VALUE - mMaximum)
		{
			mMaximums.push_back(mMinimum);
		}
		else
		{
			mMaximums.push_back(mMaximum);
		}
	}
	else
	{
		mMaximums.push_back(mMaximum);
		mMinimums.push_back(mMinimum);
		mAverageMaximums.push_back(mAverageMaximum);
		mAverageMinimums.push_back(mAverageMinimum);
		mAverages.push_back(mAverage);
	}
	mMutex.unlock();
	reset();
}

qint32 ViWaveForm::size(qint16 level)
{
	if(level != mLevel)
	{
		return mNextLevel->size(level);
	}
	mMutex.lock();
	qint32 size = mMaximums.size();
	mMutex.unlock();
	return size;
}

qint64 ViWaveForm::samples()
{
	return mMaximums.size() * FIRST_ZOOM_LEVEL;
}

unsigned char ViWaveForm::maximum(qint32 position, qint16 level)
{
	if(level != mLevel)
	{
		return mNextLevel->maximum(position, level);
	}
	mMutex.lock();
	unsigned char result = mMaximums[position];
	mMutex.unlock();
	return result;
}

unsigned char ViWaveForm::minimum(qint32 position, qint16 level)
{
	if(level != mLevel)
	{
		return mNextLevel->minimum(position, level);
	}
	mMutex.lock();
	unsigned char result = mMinimums[position];
	mMutex.unlock();
	return result;
}

unsigned char ViWaveForm::maximumAverage(qint32 position, qint16 level)
{
	if(level != mLevel)
	{
		return mNextLevel->maximumAverage(position, level);
	}
	mMutex.lock();
	unsigned char result = mAverageMaximums[position];
	mMutex.unlock();
	return result;
}

unsigned char ViWaveForm::minimumAverage(qint32 position, qint16 level)
{
	if(level != mLevel)
	{
		return mNextLevel->minimumAverage(position, level);
	}
	mMutex.lock();
	unsigned char result = mAverageMinimums[position];
	mMutex.unlock();
	return result;
}

unsigned char ViWaveForm::average(qint32 position, qint16 level)
{
	if(level != mLevel)
	{
		return mNextLevel->average(position, level);
	}
	mMutex.lock();
	unsigned char result = mAverages[position];
	mMutex.unlock();
	return result;
}

void ViWaveForm::clear()
{
	reset();
	mMaximums.clear();
	mMinimums.clear();
	mAverageMaximums.clear();
	mAverageMinimums.clear();
	mAverages.clear();
	if(mNextLevel != NULL)
	{
		mNextLevel->clear();
		mNextLevel->reset();
	}
}

bool ViWaveForm::isUnderCutoff(qint16 level)
{
	if(level != mLevel)
	{
		return mNextLevel->isUnderCutoff(level);
	}
	mMutex.lock();
	bool result = mIsUnderCutoff;
	mMutex.unlock();
	return result;
}

bool ViWaveForm::isEmpty()
{
	return size(0) == 0;
}

void ViWaveForm::reset()
{
	mMaximum = 0;
	mMinimum = 0;
	mAverageMaximum = 0;
	mAverageMinimum = 0;
	mAverage = 0;
	mMaximumCounter = 0;
	mMinimumCounter = 0;
	mTotalCounter = 0;
}

