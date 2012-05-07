#include "vicrosscorrelationmatcher.h"

ViCrossCorrelationMatcher::ViCrossCorrelationMatcher()
	: ViMatcherStrategy()
{
	mMaximumDifference = 0;
	mMinimumDifference = 1;
	mAverageDifference = 0;
	mSampleCounter = 0;
	mFirstSize = 0;
	mSecondSize = 0;
	mFirstRawData = NULL;
	mSecondRawData = NULL;
	mFirstRealData = NULL;
	mSecondRealData = NULL;
}

ViCrossCorrelationMatcher::~ViCrossCorrelationMatcher()
{
	if(mFirstRawData != NULL)
	{
		delete mFirstRawData;
	}
	if(mSecondRawData != NULL)
	{
		delete mSecondRawData;
	}
	if(mFirstRealData != NULL)
	{
		delete mFirstRealData;
	}
	if(mSecondRealData != NULL)
	{
		delete mSecondRealData;
	}
}

void ViCrossCorrelationMatcher::match()
{
	static const qint32 WINDOW_SIZE = 4096;

	qint32 firstSampleSize = mFirstStream->buffer()->format().sampleSize(), secondSampleSize = mSecondStream->buffer()->format().sampleSize();
	mFirstSize = WINDOW_SIZE * (firstSampleSize / 8), mSecondSize = WINDOW_SIZE * (secondSampleSize / 8);

	if(mFirstRawData != NULL)
	{
		delete mFirstRawData;
	}
	if(mSecondRawData != NULL)
	{
		delete mSecondRawData;
	}
	if(mFirstRealData != NULL)
	{
		delete mFirstRealData;
	}
	if(mSecondRealData != NULL)
	{
		delete mSecondRealData;
	}

	mFirstRawData = new char[mFirstSize];
	mSecondRawData = new char[mSecondSize];
	mFirstRealData = new float[mFirstSize];
	mSecondRealData = new float[mSecondSize];

	if(firstSampleSize == 8)
	{
		pcmToRealFirstPointer = &ViPcmConverter<float>::pcmToReal8;
	}
	else if(firstSampleSize == 16)
	{
		pcmToRealFirstPointer = &ViPcmConverter<float>::pcmToReal16;
	}
	else if(firstSampleSize == 24)
	{
		pcmToRealFirstPointer = &ViPcmConverter<float>::pcmToReal24;
	}
	else if(firstSampleSize == 32)
	{
		pcmToRealFirstPointer = &ViPcmConverter<float>::pcmToReal32;
	}

	if(secondSampleSize == 8)
	{
		pcmToRealSecondPointer = &ViPcmConverter<float>::pcmToReal8;
	}
	else if(secondSampleSize == 16)
	{
		pcmToRealSecondPointer = &ViPcmConverter<float>::pcmToReal16;
	}
	else if(secondSampleSize == 24)
	{
		pcmToRealSecondPointer = &ViPcmConverter<float>::pcmToReal24;
	}
	else if(secondSampleSize == 32)
	{
		pcmToRealSecondPointer = &ViPcmConverter<float>::pcmToReal32;
	}

	/*secondSize = mSecondStream->read(secondRawData, secondSize);
	firstSize = mFirstStream->read(firstRawData, firstSize);
	while(firstSize > 0 && secondSize > 0)
	{
		firstSampleSize = pcmToRealFirstPointer(firstRawData, firstRealData, firstSize);
		secondSampleSize = pcmToRealSecondPointer(secondRawData, secondRealData, secondSize);
		size = qMin(firstSampleSize, secondSampleSize);
		for(index = 0; index < size; ++index)
		{
			difference = qAbs(firstRealData[index] - secondRealData[index]);

			averageDifference += difference;
			if(difference < minimumDifference)
			{
				minimumDifference = difference;
			}
			else if(difference > maximumDifference)
			{
				maximumDifference = difference;
			}
		}
		sampleCounter += size;

		firstSize = mFirstStream->read(firstRawData, firstSize);
		secondSize = mSecondStream->read(secondRawData, secondSize);
	}*/
	calculateNext();

	/*averageDifference /= sampleCounter;
	mResult->setSampleDifference(ViMatchResultCombination(minimumDifference, maximumDifference, averageDifference));*/
}

void ViCrossCorrelationMatcher::calculateNext()
{
	static qreal difference;
	static qint32 size, index;
	static qint32 firstSampleSize, secondSampleSize;
	mSecondSize = mSecondStream->read(mSecondRawData, mSecondSize);
	mFirstSize = mFirstStream->read(mFirstRawData, mFirstSize);
	if(mFirstSize > 0 && mSecondSize > 0)
	{
		firstSampleSize = pcmToRealFirstPointer(mFirstRawData, mFirstRealData, mFirstSize);
		secondSampleSize = pcmToRealSecondPointer(mSecondRawData, mSecondRealData, mSecondSize);
		size = qMin(firstSampleSize, secondSampleSize);
		for(index = 0; index < size; ++index)
		{
			difference = qAbs(mFirstRealData[index] - mSecondRealData[index]);

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
		mSampleCounter += size;
	}
	else
	{
		mAverageDifference /= mSampleCounter;
		mResult->setCrossCorrelation(ViMatchResultCombination(mMinimumDifference, mMaximumDifference, mAverageDifference));
	}
}
