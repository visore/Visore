#include "vicomparer.h"
#include "vipcmconverter.h"
#include "math.h"

#include <iostream>
using namespace std;

ViComparer::ViComparer(ViAudioBuffer *originalBuffer, ViAudioBuffer *corectedBuffer)
{
	mOriginalBuffer = originalBuffer;
	mCorrectedBuffer = corectedBuffer;
}

void ViComparer::compare()
{
	static const qint32 chunkSize = 1024;
	int originalSampleSize = mOriginalBuffer->format().sampleSize();
	int correctedSampleSize = mCorrectedBuffer->format().sampleSize();

	int (*pcmToRealOriginal)(char*, double*, int);
	int (*pcmToRealCorrected)(char*, double*, int);
	if(originalSampleSize == 8)
	{
		pcmToRealOriginal = &ViPcmConverter<double>::pcmToReal8;
	}
	else if(originalSampleSize == 16)
	{
		pcmToRealOriginal = &ViPcmConverter<double>::pcmToReal16;
	}
	else if(originalSampleSize == 24)
	{
		pcmToRealOriginal = &ViPcmConverter<double>::pcmToReal24;
	}
	else if(originalSampleSize == 32)
	{
		pcmToRealOriginal = &ViPcmConverter<double>::pcmToReal32;
	}
	if(correctedSampleSize == 8)
	{
		pcmToRealCorrected = &ViPcmConverter<double>::pcmToReal8;
	}
	else if(correctedSampleSize == 16)
	{
		pcmToRealCorrected = &ViPcmConverter<double>::pcmToReal16;
	}
	else if(correctedSampleSize == 24)
	{
		pcmToRealCorrected = &ViPcmConverter<double>::pcmToReal24;
	}
	else if(correctedSampleSize == 32)
	{
		pcmToRealCorrected = &ViPcmConverter<double>::pcmToReal32;
	}

	mResult.reset();
	ViAudioBufferStream *originalStream = mOriginalBuffer->createReadStream();
	ViAudioBufferStream *correctedStream = mCorrectedBuffer->createReadStream();

	qint32 originalSize = chunkSize;
	qint32 correctedSize = chunkSize;
	char originalData[originalSize];
	char correctedData[correctedSize];

	qint32 originalSamplesSize = originalSize / (originalSampleSize / 8);
	qint32 correctedSamplesSize = correctedSize / (correctedSampleSize / 8);
	double originalSamples[originalSamplesSize];
	double correctedSamples[correctedSamplesSize];
	
	qint32 smallerSize;

	qint64 matchingSamples = 0;
	qint64 noneMatchingSamples = 0;
	qreal maximumDifference = 0;
	qreal minimumDifference = 0;
	qreal standardDeviation = 0;

	qreal originalSample;
	qreal correctedSample;
	qreal difference;
	qreal total = 0;

	do
	{
		originalSize = originalStream->read(originalData, originalSize);
		correctedSize = correctedStream->read(correctedData, correctedSize);

		originalSamplesSize = pcmToRealOriginal(originalData, originalSamples, originalSize);
		correctedSamplesSize = pcmToRealCorrected(correctedData, correctedSamples, correctedSize);
		
		smallerSize = originalSamplesSize;
		if(correctedSize < smallerSize)
		{
			smallerSize = correctedSamplesSize;
		}
		
		for(qint32 i = 0; i < smallerSize; ++i)
		{
			originalSample = originalSamples[i];
			correctedSample = correctedSamples[i];

			if(originalSample == correctedSample)
			{
				++matchingSamples;
			}
			else
			{
				++noneMatchingSamples;
			}

			difference = fabs(correctedSample - originalSample);
			if(difference > maximumDifference)
			{
				maximumDifference = difference;
			}
			else if(difference < minimumDifference)
			{
				minimumDifference = difference;
			}

			//total += 
		}

		mResult.setNumberOfMatchingSamples(matchingSamples);
		mResult.setNumberOfNoneMatchingSamples(noneMatchingSamples);
		mResult.setNumberOfSamples(matchingSamples + noneMatchingSamples);
		mResult.setMaximumDifference(maximumDifference);
		mResult.setMinimumDifference(minimumDifference);
		mResult.setStandardDeviation(standardDeviation);
		emit resultChanged(mResult);
	}
	while(originalSize > 0 && correctedSize > 0);
	
	cout<<mResult.toString().toAscii().data()<<endl;
}

ViCompareResult ViComparer::result()
{
	return mResult;
}
