#include "visignalmanipulator.h"
#include "vipcmconverter.h"
#include "vicodecmanager.h"
#include <time.h>
#include <QtCore/qmath.h>

#define WINDOW_SIZE 1024
#define RAND_HALF (RAND_MAX / 2)
#define PI 3.141592654
#define HALF_PI 1.570796327

#include <iostream>
using namespace std;

void ViSignalManipulator::createDefaultSignal(ViAudioBuffer *buffer, qint32 cycles, Type type)
{
	if(type == ViSignalManipulator::Flat)
	{
		ViSignalManipulator::createFlatSignal(buffer, cycles);
	}
	if(type == ViSignalManipulator::Tooth)
	{
		ViSignalManipulator::createToothSignal(buffer, cycles);
	}
	else if(type == ViSignalManipulator::Triangle)
	{
		ViSignalManipulator::createTriangleSignal(buffer, cycles);
	}
	else if(type == ViSignalManipulator::Mountain)
	{
		ViSignalManipulator::createMountainSignal(buffer, cycles);
	}
	else if(type == ViSignalManipulator::FlatMountain)
	{
		ViSignalManipulator::createFlatMountainSignal(buffer, cycles);
	}
	else if(type == ViSignalManipulator::Trapezoid)
	{
		ViSignalManipulator::createTrapezoidSignal(buffer, cycles);
	}
	else if(type == ViSignalManipulator::Sin)
	{
		ViSignalManipulator::createSinSignal(buffer, cycles);
	}
	else if(type == ViSignalManipulator::Cos)
	{
		ViSignalManipulator::createCosSignal(buffer, cycles);
	}
	else if(type == ViSignalManipulator::Tan)
	{
		ViSignalManipulator::createTanSignal(buffer, cycles);
	}
}

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

	int size = samples.size() * (format.sampleSize() / 8);
	char rawData[size];
	qreal sampleData[samples.size()];
	for(int i = 0; i < samples.size(); ++i)
	{
		sampleData[i] = samples[i];
	}
	size = ViPcmConverter<qreal>::realToPcm16(sampleData, rawData, samples.size());
	stream->write(rawData, size);
}

void ViSignalManipulator::createNoise(ViAudioBuffer *input, ViAudioBuffer *output, qreal percentage)
{
	ViAudioBufferStream *readStream = input->createReadStream();
	ViAudioBufferStream *writeStream = output->createWriteStream();
	
	srand(time(NULL));
	qreal randomMax = RAND_MAX / percentage;

	int index;
	int sampleSize = input->format().sampleSize();
	int size = WINDOW_SIZE * (sampleSize / 8);
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
			random = (sampleData[index] * rand() / randomMax);
			sign = rand();
			if(sign > RAND_HALF)
			{
				random *= -1;
			}
			random += sampleData[index];
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

void ViSignalManipulator::createFlatSignal(ViAudioBuffer *buffer, qint32 cycles)
{
	QList<qreal> list;
	static const qreal width = 100;
	
	for(int i = 0; i < cycles; ++i)
	{
		for(int j = 0; j < width; ++j)
		{
			list << 0;
		}
	}
list<<1;
	ViSignalManipulator::createSignal(list, buffer);
}

void ViSignalManipulator::createToothSignal(ViAudioBuffer *buffer, qint32 cycles)
{
	QList<qreal> list;
	static const qreal width = 200;
	static const qreal width2 = width + (width / 2);
	static const qreal width3 = (width * 3) - (width / 4);
	static const qreal width4 = (width * 3) + (width / 4);
	static const qreal offset = qSin((width2) / width * HALF_PI) - qSin((width3) / width * HALF_PI);
	for(int i = 0; i < cycles; ++i)
	{
		for(int j = 0; j < width2; ++j)
		{
			list << qSin(j / width * HALF_PI);
		}
		for(int j = width3; j < width4; ++j)
		{
			list << offset + qSin(j / width * HALF_PI);
		}
		for(int j = width2; j >= 0; --j)
		{
			list << qSin(j / width * HALF_PI);
		}
	}
	ViSignalManipulator::createSignal(list, buffer);
}

void ViSignalManipulator::createTriangleSignal(ViAudioBuffer *buffer, qint32 cycles)
{
	QList<qreal> list;
	static const qreal width = 500;
	for(int i = 0; i < cycles; ++i)
	{
		for(int j = 0; j < width; ++j)
		{
			list << j / width;
		}
		for(int j = width; j >= 0; --j)
		{
			list << j / width;
		}
	}
	ViSignalManipulator::createSignal(list, buffer);
}

void ViSignalManipulator::createMountainSignal(ViAudioBuffer *buffer, qint32 cycles)
{
	QList<qreal> list;
	static const qreal width = 200;
	static const qreal width2 = 150;
	static const qreal width3 = width + width2;
	static const qreal ratio = width2 / width;
	static const qreal ratio2 = width / width3;
	for(int i = 0; i < cycles; ++i)
	{
		for(int j = 0; j < width; ++j)
		{
			list << j / (width3);
		}
		for(int j = 0; j < width2; ++j)
		{
			list << ratio2 - (j / width3) * ratio;
		}
		for(int j = width2; j >= 0; --j)
		{
			list << ratio2 - (j / width3) * ratio;
		}
		for(int j = width; j >= 0; --j)
		{
			list << j / width3;
		}
	}
	ViSignalManipulator::createSignal(list, buffer);
}

void ViSignalManipulator::createFlatMountainSignal(ViAudioBuffer *buffer, qint32 cycles)
{
	QList<qreal> list;
	static const qreal width = 75;
	static const qreal width2 = 125;
	static const qreal width3 = 250;
	static const qreal width4 = 350;
	static const qreal width5 = 450;
	static const qreal width6 = 625;
	static const qreal flat = width / width2;
	static const qreal gradient = 0.2;
	static const qreal gap = width4 - width3;
	qreal w = width3;
	for(int i = 0; i < cycles; ++i)
	{
		for(int j = 0; j < width; ++j)
		{
			list << j / width2;
		}
		for(int j = width; j < width3; ++j)
		{
			list << flat;
		}
		for(int j = gap; j >= 0; --j)
		{
			list << (flat - gradient) + (gradient * j / gap);
		}
		for(int j = 0; j < gap; ++j)
		{
			list << (flat - gradient) + (gradient * j / gap);
		}
		for(int j = width5; j < width6; ++j)
		{
			list << flat;
		}
		for(int j = width; j >= 0; --j)
		{
			list << j / width2;
		}
	}
	ViSignalManipulator::createSignal(list, buffer);
}

void ViSignalManipulator::createTrapezoidSignal(ViAudioBuffer *buffer, qint32 cycles)
{
	QList<qreal> list;
	static const qreal width = 75;
	static const qreal width2 = 125;
	static const qreal width3 = 625;
	static const qreal flat = width / width2;
	for(int i = 0; i < cycles; ++i)
	{
		for(int j = 0; j < width; ++j)
		{
			list << j / width2;
		}
		for(int j = width; j < width3; ++j)
		{
			list << flat;
		}
		for(int j = width; j >= 0; --j)
		{
			list << j / width2;
		}
	}
	ViSignalManipulator::createSignal(list, buffer);
}

void ViSignalManipulator::createSinSignal(ViAudioBuffer *buffer, qint32 cycles)
{
	QList<qreal> list;
	static const qreal width = 200;
	for(int i = 0; i < cycles; ++i)
	{
		for(int j = 0; j < width; ++j)
		{
			list << qSin(j / width * 2 * PI);
		}
	}
	ViSignalManipulator::createSignal(list, buffer);
}

void ViSignalManipulator::createCosSignal(ViAudioBuffer *buffer, qint32 cycles)
{
	QList<qreal> list;
	static const qreal width = 200;
	for(int i = 0; i < cycles; ++i)
	{
		for(int j = 0; j < width; ++j)
		{
			list << qCos(j / width * 2 * PI);
		}
	}
	ViSignalManipulator::createSignal(list, buffer);
}

void ViSignalManipulator::createTanSignal(ViAudioBuffer *buffer, qint32 cycles)
{
	QList<qreal> list;
	static const qreal width = 200;
	for(int i = 0; i < cycles; ++i)
	{
		for(int j = 0; j < width; ++j)
		{
			list << qTan(j / width * 2 * PI);
		}
	}
	ViSignalManipulator::createSignal(list, buffer);
}
