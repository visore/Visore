#include "visamplecorrelator.h"
#include <float.h>

ViSampleCorrelator::ViSampleCorrelator()
	: ViCorrelatorStrategy()
{
}

void ViSampleCorrelator::initialize()
{
	mMaximumDifference = -DBL_MAX;
	mMinimumDifference = DBL_MAX;
	mAverageDifference = 0;
	mCounter = 0;
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

void ViSampleCorrelator::execute()
{
	qreal difference;
	qreal *data = mData->data();
	qreal *data2 = mData2->data();
	qint32 size = qMin(mData->size(), mData2->size());
	for(int i = 0; i < size; ++i)
	{
		difference = qAbs(data[i] - data2[i]);
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
