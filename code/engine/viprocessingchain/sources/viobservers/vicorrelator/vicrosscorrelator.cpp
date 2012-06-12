#include "vicrosscorrelator.h"
#include <float.h>

ViCrossCorrelator::ViCrossCorrelator()
	: ViCorrelatorStrategy()
{
	mRealData = NULL;
	mFirstFourierData = NULL;
	mSecondFourierData = NULL;
	mMultiplyData = NULL;
	mAutocorrelationData = NULL;
}

void ViCrossCorrelator::initialize(qint32 windowSize)
{
	ViCorrelatorStrategy::initialize(windowSize);
	mTransformer.setSize(mWindowSize);
	allocateData();

	mMaximumDifference = -DBL_MAX;
	mMinimumDifference = DBL_MAX;
	mAverageDifference = 0;
	mCounter = 0;
}

void ViCrossCorrelator::run()
{
	qreal subAverageDifference, subSampleCounter, difference, firstNorm, secondNorm, multipliedNorm;

	memcpy(mRealData, mFirstData, mFirstSize * sizeof(qreal));
	firstNorm = norm(mRealData, mFirstSize);
	multipliedNorm = firstNorm * firstNorm;
	mTransformer.pad(mRealData, mFirstSize);
	mTransformer.forwardTransform(mRealData, mFirstFourierData);

	//Auto-correlation
	memcpy(mRealData, mFirstFourierData, mWindowSize * sizeof(qreal));
	mTransformer.conjugate(mFirstFourierData);
	mTransformer.multiply(mFirstFourierData, mRealData, mMultiplyData);	
	mTransformer.inverseTransform(mMultiplyData, mAutocorrelationData);
	mTransformer.rescale(mAutocorrelationData);
	applyNorm(mAutocorrelationData, mWindowSize, multipliedNorm);

	//Cross-correlation
	memcpy(mRealData, mSecondData, mSecondSize * sizeof(qreal));
	secondNorm = norm(mRealData, mSecondSize);
	multipliedNorm = firstNorm * secondNorm;
	mTransformer.pad(mRealData, mSecondSize);
	mTransformer.forwardTransform(mRealData, mSecondFourierData);
	mTransformer.multiply(mFirstFourierData, mSecondFourierData, mMultiplyData);
	mTransformer.inverseTransform(mMultiplyData, mRealData);
	mTransformer.rescale(mRealData);
	applyNorm(mRealData, mWindowSize, multipliedNorm);

	subAverageDifference = 0;
	subSampleCounter = 0;
	for(int i = 0; i < mWindowSize; ++i)
	{
		difference = qAbs(mRealData[i] - mAutocorrelationData[i]);
		if(difference > mMaximumDifference)
		{
			mMaximumDifference = difference;
		}
		else if(difference < mMinimumDifference)
		{
			mMinimumDifference = difference;
		}
		subAverageDifference += difference;
		++subSampleCounter;
	}
	//mAverageDifference += (subAverageDifference / subSampleCounter);
	++mCounter;
	mAverageDifference = ((mAverageDifference / mCounter) * (mCounter - 1)) + ((subAverageDifference / subSampleCounter) / mCounter);
}

void ViCrossCorrelator::finalize()
{
	deallocateData();
	/*mAverageDifference /= mCounter;
	mResult->setCrossCorrelation(ViCorrelationResultCombination(
		(2 - qAbs(mMaximumDifference)) / 2,
		(2 - qAbs(mMinimumDifference)) / 2,
		(2 - qAbs(mAverageDifference)) / 2
	));*/
}

void ViCrossCorrelator::allocateData()
{
	mRealData = new qreal[mWindowSize];
	mFirstFourierData = new qreal[mWindowSize];
	mSecondFourierData = new qreal[mWindowSize];
	mMultiplyData = new qreal[mWindowSize];
	mAutocorrelationData = new qreal[mWindowSize];
}

void ViCrossCorrelator::deallocateData()
{
	if(mRealData != NULL)
	{
		delete [] mRealData;
		mRealData = NULL;
	}
	if(mFirstFourierData != NULL)
	{
		delete [] mFirstFourierData;
		mFirstFourierData = NULL;
	}
	if(mSecondFourierData != NULL)
	{
		delete [] mSecondFourierData;
		mSecondFourierData = NULL;
	}
	if(mMultiplyData != NULL)
	{
		delete [] mMultiplyData;
		mMultiplyData = NULL;
	}
	if(mAutocorrelationData != NULL)
	{
		delete [] mAutocorrelationData;
		mAutocorrelationData = NULL;
	}
}

qreal ViCrossCorrelator::norm(qreal array[], qint32 size)
{
	qreal norm = 0;
	for(int i = 0; i < size; ++i)
	{
		norm += (array[i] * array[i]);
	}
	return qSqrt(norm);
}

void ViCrossCorrelator::applyNorm(qreal array[], qint32 size, qreal norm)
{
	if(norm != 0)
	{
		for(int i = 0; i < size; ++i)
		{
			array[i] /= norm;
		}
	}
}
