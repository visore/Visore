#include "visamplematcher.h"

#define WINDOW_SIZE 4096

ViSampleMatcher::ViSampleMatcher()
	: ViMatcherStrategy()
{
}

void ViSampleMatcher::match()
{
	qreal maximumDifference = 0;
	qreal minimumDifference = 1;
	qreal averageDifference = 0;

	qreal difference;
	qint64 sampleCounter = 0;
	qint32 size, index;
	qint32 firstSize = WINDOW_SIZE, secondSize = WINDOW_SIZE;
	qint32 firstSampleSize = mFirstStream->buffer()->format().sampleSize(), secondSampleSize = mSecondStream->buffer()->format().sampleSize();

	char firstRawData[firstSize];
	char secondRawData[secondSize];
	qreal firstRealData[firstSize];
	qreal secondRealData[secondSize];

	int (*pcmToRealFirstPointer)(char*, qreal*, int);
	int (*pcmToRealSecondPointer)(char*, qreal*, int);

	if(firstSampleSize == 8)
	{
		pcmToRealFirstPointer = &ViPcmConverter<qreal>::pcmToReal8;
	}
	else if(firstSampleSize == 16)
	{
		pcmToRealFirstPointer = &ViPcmConverter<qreal>::pcmToReal16;
	}
	else if(firstSampleSize == 24)
	{
		pcmToRealFirstPointer = &ViPcmConverter<qreal>::pcmToReal24;
	}
	else if(firstSampleSize == 32)
	{
		pcmToRealFirstPointer = &ViPcmConverter<qreal>::pcmToReal32;
	}

	if(secondSampleSize == 8)
	{
		pcmToRealSecondPointer = &ViPcmConverter<qreal>::pcmToReal8;
	}
	else if(secondSampleSize == 16)
	{
		pcmToRealSecondPointer = &ViPcmConverter<qreal>::pcmToReal16;
	}
	else if(secondSampleSize == 24)
	{
		pcmToRealSecondPointer = &ViPcmConverter<qreal>::pcmToReal24;
	}
	else if(secondSampleSize == 32)
	{
		pcmToRealSecondPointer = &ViPcmConverter<qreal>::pcmToReal32;
	}

	do
	{
		firstSize = mFirstStream->read(firstRawData, firstSize);
		firstSampleSize = pcmToRealFirstPointer(firstRawData, firstRealData, firstSize);
		secondSize = mSecondStream->read(secondRawData, secondSize);
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
	}
	while(firstSize > 0 && secondSize > 0);

	averageDifference /= sampleCounter;
	mResult->setSampleDifference(ViMatchResultCombination(minimumDifference, maximumDifference, averageDifference));
}
