#include "visamplecorrelator.h"
#include <float.h>

ViSampleCorrelator::ViSampleCorrelator()
	: ViCorrelator()
{
}

void ViSampleCorrelator::exportResults(ViElement &element)
{
	//ViProcessor::exportResults(element);
	element.addChild("Maximum", mMaximumDifference);
	element.addChild("Minimum", mMinimumDifference);
	element.addChild("Average", mAverageDifference);
}

void ViSampleCorrelator::initialize()
{
	mMaximumDifference = -DBL_MAX;
	mMinimumDifference = DBL_MAX;
	mAverageDifference = 0;
	mCounter = 0;
}

void ViSampleCorrelator::execute()
{
	qreal difference;
	ViSampleChunk &theSamples1 = samples1();
	ViSampleChunk &theSamples2 = samples2();

	qint32 size = qMin(theSamples1.size(), theSamples2.size());
	for(int i = 0; i < size; ++i)
	{
		difference = qAbs(theSamples1[i] - theSamples2[i]);
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
	/*mResult.setSampleCorrelation(ViCorrelationResultCombination(
		(2 - qAbs(mMaximumDifference)) / 2,
		(2 - qAbs(mMinimumDifference)) / 2,
		(2 - qAbs(mAverageDifference)) / 2
	));*/

	ViElement element;
	//exportResults(element);
	object()->addCorrelation(element);
}
