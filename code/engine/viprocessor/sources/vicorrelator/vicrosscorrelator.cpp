#include "vicrosscorrelator.h"
#include <float.h>
#include <viaudioposition.h>

ViCrossCorrelator::ViCrossCorrelator()
	: ViCorrelator()
{
	mRealData = NULL;
	mFirstFourierData = NULL;
	mSecondFourierData = NULL;
	mMultiplyData = NULL;
	mAutocorrelationData = NULL;
}

ViCrossCorrelator::~ViCrossCorrelator()
{
	deallocateData();
}

void ViCrossCorrelator::exportResults(ViElement &element)
{
	//ViProcessor::exportResults(element);
	element.addChild("Maximum", mMaximumDifference);
	element.addChild("Minimum", mMinimumDifference);
	element.addChild("Average", mAverageDifference);
}

void ViCrossCorrelator::initialize()
{
	mTransformer.setSize(sampleCount());

	deallocateData();
	allocateData();

	mMaximumDifference = -DBL_MAX;
	mMinimumDifference = DBL_MAX;
	mAverageDifference = 0;
	mCounter = 0;
}

void ViCrossCorrelator::finalize()
{
	deallocateData();
	mAverageDifference /= mCounter;

	ViElement element;
	//exportResults(element);
	object()->addCorrelation(element);
}

void ViCrossCorrelator::execute()
{
	ViSampleChunk &theSamples1 = samples1();
	ViSampleChunk &theSamples2 = samples2();

	qreal subAverageDifference, subSampleCounter, difference, firstNorm, secondNorm, multipliedNorm;

	memcpy(mRealData, theSamples1.data(), theSamples1.size() * sizeof(qreal));
	firstNorm = norm(mRealData, theSamples1.size());
	multipliedNorm = firstNorm * firstNorm;
	mTransformer.pad(mRealData, theSamples1.size());
	mTransformer.forwardTransform(mRealData, mFirstFourierData);

	//Auto-correlation
	memcpy(mRealData, mFirstFourierData, sampleCount() * sizeof(qreal));
	mTransformer.conjugate(mFirstFourierData);
	mTransformer.multiply(mFirstFourierData, mRealData, mMultiplyData);	
	mTransformer.inverseTransform(mMultiplyData, mAutocorrelationData);
	mTransformer.rescale(mAutocorrelationData);
	applyNorm(mAutocorrelationData, sampleCount(), multipliedNorm);

	//Cross-correlation
	memcpy(mRealData, theSamples2.data(), theSamples2.size() * sizeof(qreal));
	secondNorm = norm(mRealData, theSamples2.size());
	multipliedNorm = firstNorm * secondNorm;
	mTransformer.pad(mRealData, theSamples2.size());
	mTransformer.forwardTransform(mRealData, mSecondFourierData);
	mTransformer.multiply(mFirstFourierData, mSecondFourierData, mMultiplyData);
	mTransformer.inverseTransform(mMultiplyData, mRealData);
	mTransformer.rescale(mRealData);
	//applyNorm(mRealData, sampleCount(), multipliedNorm);

	subAverageDifference = 0;
	subSampleCounter = 0;
	for(int i = 0; i < sampleCount(); ++i)
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
	mAverageDifference += (subAverageDifference / subSampleCounter);
	++mCounter;
}

void ViCrossCorrelator::allocateData()
{
	mRealData = new qreal[sampleCount()];
	mFirstFourierData = new qreal[sampleCount()];
	mSecondFourierData = new qreal[sampleCount()];
	mMultiplyData = new qreal[sampleCount()];
	mAutocorrelationData = new qreal[sampleCount()];
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
