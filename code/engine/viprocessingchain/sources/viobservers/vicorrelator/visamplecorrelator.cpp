#include "visamplecorrelator.h"
#include <float.h>

ViSampleCorrelator::ViSampleCorrelator()
	: ViCorrelatorStrategy()
{
}

void ViSampleCorrelator::initialize(qint32 windowSize)
{
	ViCorrelatorStrategy::initialize(windowSize);
	mMaximumDifference = -DBL_MAX;
	mMinimumDifference = DBL_MAX;
	mAverageDifference = 0;
	mCounter = 0;
}

void ViSampleCorrelator::run()
{
	qreal difference;
	qint32 size = qMin(mFirstSize, mSecondSize);
	for(int i = 0; i < size; ++i)
	{
		difference = qAbs(mFirstData[i] - mSecondData[i]);
if(difference != NULL)
{
	cout<<difference<<" "<<mFirstData[i]<<" "<<mSecondData[i]<<endl;
}

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

void ViSampleCorrelator::finalize()
{
	mAverageDifference /= mCounter;
	mResult->setSampleCorrelation(ViCorrelationResultCombination(
		(2 - qAbs(mMaximumDifference)) / 2,
		(2 - qAbs(mMinimumDifference)) / 2,
		(2 - qAbs(mAverageDifference)) / 2
	));
}
