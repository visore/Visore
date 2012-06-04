#include "vicrosscorrelationmatcher.h"
#include <float.h>

#include <iostream>
using namespace std;

ViCrossCorrelationMatcher::ViCrossCorrelationMatcher()
	: ViMatcherStrategy()
{
}

void ViCrossCorrelationMatcher::match()
{
	static const qint32 WINDOW_SIZE = 4096;
	int (*pcmToRealFirstPointer)(char*, double*, int);
	int (*pcmToRealSecondPointer)(char*, double*, int);

	qreal maximumDifference = -DBL_MAX;
	qreal minimumDifference = DBL_MAX;
	qreal averageDifference = 0, subAverageDifference;
	qreal sampleCounter = 0, subSampleCounter;
	qreal difference, firstNorm, secondNorm, multipliedNorm;

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
			firstNorm = norm(realData, firstSampleSize);
			multipliedNorm = firstNorm * firstNorm;
			transformer.pad(realData, firstSampleSize);
			transformer.forwardTransform(realData, firstFourierData);

			//Auto-correlation
			memcpy(realData, firstFourierData, WINDOW_SIZE * sizeof(double));
			transformer.conjugate(firstFourierData);
			transformer.multiply(firstFourierData, realData, multiply);	
			transformer.inverseTransform(multiply, autocorrelation);
			transformer.rescale(autocorrelation);
			applyNorm(autocorrelation, WINDOW_SIZE, multipliedNorm);

			//Cross-correlation
			secondSampleSize = pcmToRealSecondPointer(secondRawData, realData, secondSize);
			secondNorm = norm(realData, secondSampleSize);
			multipliedNorm = firstNorm * secondNorm;
			transformer.pad(realData, secondSampleSize);
			transformer.forwardTransform(realData, secondFourierData);
			transformer.multiply(firstFourierData, secondFourierData, multiply);
			transformer.inverseTransform(multiply, realData);
			transformer.rescale(realData);
			applyNorm(realData, WINDOW_SIZE, multipliedNorm);

			subAverageDifference = 0;
			subSampleCounter = 0;
			for(int i = 0; i < WINDOW_SIZE; ++i)
			{
				difference = 1 - qAbs(realData[i] - autocorrelation[i]);
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

qreal ViCrossCorrelationMatcher::norm(qreal array[], qint32 size)
{
	qreal norm = 0;
	for(int i = 0; i < size; ++i)
	{
		norm += (array[i] * array[i]);
	}
	return qSqrt(norm);
}

void ViCrossCorrelationMatcher::applyNorm(qreal array[], qint32 size, qreal norm)
{
	if(norm != 0)
	{
		for(int i = 0; i < size; ++i)
		{
			array[i] /= norm;
		}
	}
}
