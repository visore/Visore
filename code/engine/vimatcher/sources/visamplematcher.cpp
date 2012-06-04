#include "visamplematcher.h"
#include <float.h>

ViSampleMatcher::ViSampleMatcher()
	: ViMatcherStrategy()
{
}

void ViSampleMatcher::initialize(qint32 windowSize)
{
	ViMatcherStrategy::initialize(windowSize);
	mMaximumDifference = -DBL_MAX;
	mMinimumDifference = DBL_MAX;
	mAverageDifference = 0;
	mCounter = 0;
}

void ViSampleMatcher::run()
{
	qreal difference;
	qint32 size = qMin(mFirstSize, mSecondSize);
	for(int i = 0; i < size; ++i)
	{
		difference = qAbs(mFirstData[i] - mSecondData[i]);

		mAverageDifference += difference;
		if(difference < mMinimumDifference)
		{
			mMinimumDifference = difference;
		}
		else if(difference > mMaximumDifference)
		{
			mMaximumDifference = difference;
		}
	}
	mCounter += size;
}

void ViSampleMatcher::finalize()
{
	mAverageDifference /= mCounter;
	mResult->setSampleDifference(ViMatchResultCombination(
		(2 - qAbs(mMaximumDifference)) / 2,
		(2 - qAbs(mMinimumDifference)) / 2,
		(2 - qAbs(mAverageDifference)) / 2
	));
}
