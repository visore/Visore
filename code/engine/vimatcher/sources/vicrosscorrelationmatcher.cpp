#include "vicrosscorrelationmatcher.h"

#include <iostream>
using namespace std;

ViCrossCorrelationMatcher::ViCrossCorrelationMatcher()
	: ViMatcherStrategy()
{
	mMaximumDifference = 0;
	mMinimumDifference = 1;
	mAverageDifference = 0;
	mSampleCounter = 0;
	mFirstSize = 0;
	mSecondSize = 0;
	mFirstSampleSize = 0;
	mSecondSampleSize = 0;
	mFirstRawData = NULL;
	mSecondRawData = NULL;
	mFirstRealData = NULL;
	mSecondRealData = NULL;
	mFirstFourierData = NULL;
	mSecondFourierData = NULL;

	QObject::connect(&mTransformer1, SIGNAL(finished()), this, SLOT(summarize()));
	//QObject::connect(&mTransformer2, SIGNAL(finished()), this, SLOT(summarize()));
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
	if(mFirstFourierData != NULL)
	{
		delete mFirstFourierData;
	}
	if(mSecondFourierData != NULL)
	{
		delete mSecondFourierData;
	}
}

void ViCrossCorrelationMatcher::match()
{
	static const qint32 WINDOW_SIZE = 4096;
	mFirstFourierFinished = false;

	mFirstSampleSize = mFirstStream->buffer()->format().sampleSize();
	mSecondSampleSize = mSecondStream->buffer()->format().sampleSize();
	mFirstSize = WINDOW_SIZE * (mFirstSampleSize / 8);
	mSecondSize = WINDOW_SIZE * (mSecondSampleSize / 8);

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
	if(mFirstFourierData != NULL)
	{
		delete mFirstFourierData;
	}
	if(mSecondFourierData != NULL)
	{
		delete mSecondFourierData;
	}

	mFirstRawData = new char[mFirstSize];
	mSecondRawData = new char[mSecondSize];
	mFirstRealData = new float[mFirstSize];
	mSecondRealData = new float[mSecondSize];
	mFirstFourierData = new float[mFirstSize];
	mSecondFourierData = new float[mSecondSize];

	if(mFirstSampleSize == 8)
	{
		pcmToRealFirstPointer = &ViPcmConverter<float>::pcmToReal8;
	}
	else if(mFirstSampleSize == 16)
	{
		pcmToRealFirstPointer = &ViPcmConverter<float>::pcmToReal16;
	}
	else if(mFirstSampleSize == 24)
	{
		pcmToRealFirstPointer = &ViPcmConverter<float>::pcmToReal24;
	}
	else if(mFirstSampleSize == 32)
	{
		pcmToRealFirstPointer = &ViPcmConverter<float>::pcmToReal32;
	}

	if(mSecondSampleSize == 8)
	{
		pcmToRealSecondPointer = &ViPcmConverter<float>::pcmToReal8;
	}
	else if(mSecondSampleSize == 16)
	{
		pcmToRealSecondPointer = &ViPcmConverter<float>::pcmToReal16;
	}
	else if(mSecondSampleSize == 24)
	{
		pcmToRealSecondPointer = &ViPcmConverter<float>::pcmToReal24;
	}
	else if(mSecondSampleSize == 32)
	{
		pcmToRealSecondPointer = &ViPcmConverter<float>::pcmToReal32;
	}

	/*secondSize = mSecondStream->read(secondRawData, secondSize);
	firstSize = mFirstStream->read(firstRawData, firstSize);
	while(firstSize > 0 && secondSize > 0)
	{
		mFirstSampleSize = pcmToRealFirstPointer(firstRawData, firstRealData, firstSize);
		mSecondSampleSize = pcmToRealSecondPointer(secondRawData, secondRealData, secondSize);
		size = qMin(mFirstSampleSize, mSecondSampleSize);
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
	mFirstSize = mFirstStream->read(mFirstRawData, mFirstSize);
	mSecondSize = mSecondStream->read(mSecondRawData, mSecondSize);
	if(mFirstSize > 0 && mSecondSize > 0)
	{
		mFirstSampleSize = pcmToRealFirstPointer(mFirstRawData, mFirstRealData, mFirstSize);
		mSecondSampleSize = pcmToRealSecondPointer(mSecondRawData, mSecondRealData, mSecondSize);
/*
for(int i = 0; i < mFirstSampleSize; ++i)cout<<mFirstRealData[i]<<" ";
cout<<endl;*/
for(int i = 0; i < mFirstSize; ++i)mFirstFourierData[i]=0;

		mTransformer1.forwardTransform(mFirstFourierData, mFirstRealData, mFirstSampleSize);
		//mTransformer2.forwardTransform(mSecondFourierData, mSecondRealData, mSecondSampleSize);
	}
	else
	{
		mAverageDifference /= mSampleCounter;
		mResult->setCrossCorrelation(ViMatchResultCombination(mMinimumDifference, mMaximumDifference, mAverageDifference));
	}
}

void ViCrossCorrelationMatcher::summarize()
{
	mFirstFourierFinished = !mFirstFourierFinished;
	static qreal difference;
	static qint32 size, index;
	size = qMin(mFirstSampleSize, mSecondSampleSize);
size=mFirstSampleSize;
cout<<"finished"<<endl;
	for(index = 0; index < size; ++index)
	{

if(mFirstFourierData[index] != 0)
cout<<mFirstFourierData[index]<<endl;

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
