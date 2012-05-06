#include "visignalmanipulator.h"
#include "vipcmconverter.h"
#include <time.h>

#define WINDOW_SIZE 1024
#define RAND_HALF (RAND_MAX / 2)

void ViSignalManipulator::createSignal(QList<qreal> samples, ViAudioBuffer *buffer)
{
	buffer->clear();
	ViAudioBufferStream *stream = buffer->createWriteStream();
	ViAudioFormat format;
	format.setSampleRate(44100);
	format.setChannelCount(1);
	format.setSampleSize(16);
	format.setCodec(ViCodecManager::selected("PCM"));
	format.setSampleType(QAudioFormat::SignedInt);
	format.setByteOrder(QAudioFormat::LittleEndian);
	buffer->setFormat(format);

	int size = samples.size() * format.sampleSize();
	char rawData[size];
	qreal sampleData[samples.size()];
	for(int i = 0; i < samples.size(); ++i)
	{
		sampleData[i] = samples[i];
	}
	size = ViPcmConverter::realToPcm16(sampleData, rawData, size);
	stream->write(rawData, size);
}

void ViSignalManipulator::addRandomNoise(ViAudioBuffer *buffer, qreal percentage)
{
	ViAudioBufferStream *readStream = buffer->createWriteStream();
	ViAudioBufferStream *writeStream = buffer->createWriteStream();
	
	srand(time(NULL));
	int randomMax = RAND_MAX * precentage;

	int index;
	int sampleSize = buffer->format().sampleSize();
	int size = WINDOW_SIZE * sampleSize;
	qreal random;
	int sign;

	char rawData[size];
	qreal sampleData[WINDOW_SIZE];
	size = readStream->read(rawData, size);

	int (*pcmToRealPointer)(char*, qreal*, int);
	int (*realToPcmPointer)(qreal*, char*, int);
	if(sampleSize == 8)
	{
		pcmToRealPointer = &ViPcmConverter<qreal>::pcmToReal8;
		realToPcmPointer = &ViPcmConverter<qreal>::realToPcm8;
	}
	else if(sampleSize == 16)
	{
		pcmToRealPointer = &ViPcmConverter<qreal>::pcmToReal16;
		realToPcmPointer = &ViPcmConverter<qreal>::realToPcm16;
	}
	else if(sampleSize == 24)
	{
		pcmToRealPointer = &ViPcmConverter<qreal>::pcmToReal24;
		realToPcmPointer = &ViPcmConverter<qreal>::realToPcm24;
	}
	else if(sampleSize == 32)
	{
		pcmToRealPointer = &ViPcmConverter<qreal>::pcmToReal32;
		realToPcmPointer = &ViPcmConverter<qreal>::realToPcm32;
	}

	while(size > 0)
	{
		size = pcmToRealPointer(rawData, sampleData, size);
		for(index = 0; index < size; ++index)
		{
			random = sampleData[index] + (rand() / randomMax);
			sign = rand();
			if(sign > RAND_HALF)
			{
				random *= -1;
			}
			if(random > 1)
			{
				random = 1;
			}
			else if(random < -1)
			{
				random = -1;
			}
			sampleData[index] = random;
		}
		size = realToPcmPointer(sampleData, rawData, size);
		writeStream->write(rawData, size);
		size = readStream->read(rawData, size);
	}
}
