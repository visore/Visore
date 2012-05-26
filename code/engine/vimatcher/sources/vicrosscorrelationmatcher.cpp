#include "vicrosscorrelationmatcher.h"
#include <float.h>

#include <iostream>
using namespace std;

ViCrossCorrelationMatcherThread::ViCrossCorrelationMatcherThread()
{
	mFirstStream = NULL;
	mSecondStream = NULL;
	mResult = NULL;
}

void ViCrossCorrelationMatcherThread::setStreams(ViAudioBufferStream *first, ViAudioBufferStream *second)
{
	mFirstStream = first;
	mSecondStream = second;
}

void ViCrossCorrelationMatcherThread::setResult(ViMatchResult *result)
{
	mResult = result;
}

void ViCrossCorrelationMatcherThread::run()
{
	static const qint32 WINDOW_SIZE = 4096;
	int (*pcmToRealFirstPointer)(char*, double*, int);
	int (*pcmToRealSecondPointer)(char*, double*, int);

	qreal maximumDifference = -DBL_MIN;
	qreal minimumDifference = DBL_MIN;
	qreal averageDifference = 0, subAverageDifference;
	qreal sampleCounter = 0, subSampleCounter;
	qreal difference;

	mFirstStream->restart();
	mSecondStream->restart();

	ViFourierTransformer transformer;
	transformer.setSize(WINDOW_SIZE);

	qint32 firstSampleSize = mFirstStream->buffer()->format().sampleSize();
	qint32 secondSampleSize = mSecondStream->buffer()->format().sampleSize();
	qint32 halfSize = WINDOW_SIZE / 2;
	qint32 firstSize = halfSize * (firstSampleSize / 8);
	qint32 secondSize = halfSize * (secondSampleSize / 8);

	char firstRawData[firstSize];
	char secondRawData[secondSize];
	double realData[WINDOW_SIZE];
	double firstFourierData[WINDOW_SIZE];
	double secondFourierData[WINDOW_SIZE];
	double multiply[WINDOW_SIZE];
	double autocorrelation[WINDOW_SIZE];

	if(firstSampleSize == 8)
	{
		pcmToRealFirstPointer = &ViPcmConverter<double>::pcmToReal8;
	}
	else if(firstSampleSize == 16)
	{
		pcmToRealFirstPointer = &ViPcmConverter<double>::pcmToReal16;
	}
	else if(firstSampleSize == 24)
	{
		pcmToRealFirstPointer = &ViPcmConverter<double>::pcmToReal24;
	}
	else if(firstSampleSize == 32)
	{
		pcmToRealFirstPointer = &ViPcmConverter<double>::pcmToReal32;
	}

	if(secondSampleSize == 8)
	{
		pcmToRealSecondPointer = &ViPcmConverter<double>::pcmToReal8;
	}
	else if(secondSampleSize == 16)
	{
		pcmToRealSecondPointer = &ViPcmConverter<double>::pcmToReal16;
	}
	else if(secondSampleSize == 24)
	{
		pcmToRealSecondPointer = &ViPcmConverter<double>::pcmToReal24;
	}
	else if(secondSampleSize == 32)
	{
		pcmToRealSecondPointer = &ViPcmConverter<double>::pcmToReal32;
	}

	while(firstSize > 0 && secondSize > 0)
	{
		firstSize = mFirstStream->read(firstRawData, firstSize);
		secondSize = mSecondStream->read(secondRawData, secondSize);

		if(firstSize > 0 && secondSize > 0)
		{
			firstSampleSize = pcmToRealFirstPointer(firstRawData, realData, firstSize);
			transformer.pad(realData, firstSampleSize);
			transformer.forwardTransform(realData, firstFourierData);

			//Auto-correlation
			memcpy(realData, firstFourierData, WINDOW_SIZE * sizeof(double));
			transformer.conjugate(firstFourierData);
			transformer.multiply(firstFourierData, realData, multiply);	
			transformer.inverseTransform(multiply, autocorrelation);
			transformer.rescale(autocorrelation);

			//Cross-correlation
			secondSampleSize = pcmToRealSecondPointer(secondRawData, realData, secondSize);
			transformer.pad(realData, secondSampleSize);
			transformer.forwardTransform(realData, secondFourierData);
			transformer.multiply(firstFourierData, secondFourierData, multiply);
			transformer.inverseTransform(multiply, realData);
			transformer.rescale(realData);

			subAverageDifference = 0;
			subSampleCounter = 0;
			for(int i = 0; i < WINDOW_SIZE; ++i)
			{
				difference = qAbs(realData[i] - autocorrelation[i]);
				if(difference > maximumDifference)
				{
					maximumDifference = difference;
				}
				else if(difference < minimumDifference)
				{
					minimumDifference = difference;
				}
				subAverageDifference += difference;
				++subSampleCounter;
			}
			averageDifference += (subAverageDifference / subSampleCounter);
			++sampleCounter;
		}
		else
		{
			averageDifference /= sampleCounter;
			mResult->setCrossCorrelation(ViMatchResultCombination(minimumDifference, maximumDifference, averageDifference));
		}
	}
}

ViCrossCorrelationMatcher::ViCrossCorrelationMatcher()
	: ViMatcherStrategy()
{
	QObject::connect(&mThread, SIGNAL(finished()), this, SIGNAL(finished()));
}

void ViCrossCorrelationMatcher::match()
{
	mThread.setStreams(mFirstStream, mSecondStream);
	mThread.setResult(mResult);
	mThread.start();
}

/*
void ViCrossCorrelationMatcher::summarize()
{
	mFirstFourierFinished = !mFirstFourierFinished;
	static qreal difference;
	static qint32 size, index;
	size = qMin(mFirstSampleSize, mSecondSampleSize);
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
}*/
