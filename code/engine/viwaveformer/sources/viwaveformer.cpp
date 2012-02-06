#include "viwaveformer.h"

#include <iostream>
using namespace std;

ViWaveFormerThread::ViWaveFormerThread(ViAudioBuffer *buffer, ViAudioMetaData *metaData, QList<int> *sizes)
	: ViProcessorThread(buffer, sizes)
{
	mMetaData = metaData;
	if(mMetaData->bitDepth() == 8)
	{
		pcmToReal = &ViWaveFormerThread::pcmToReal8;
	}
	else if(mMetaData->bitDepth() == 32)
	{
		pcmToReal = &ViWaveFormerThread::pcmToReal32;
	}
	else
	{
		pcmToReal = &ViWaveFormerThread::pcmToReal16;
	}
}

void ViWaveFormerThread::run()
{
	while(!mSizes->isEmpty())
	{
		int size = mSizes->first();
		mSizes->removeFirst();
		ViAudioBufferChunk chunk;
		size = mReadStream->read(&chunk, size);
		qint16 *base = reinterpret_cast<qint16*>(chunk.data());
		QList<double> result = (this->*pcmToReal)(chunk.data(), size);
		emit completed(result);
	}
}

QList<double> ViWaveFormerThread::pcmToReal8(char* buffer, int size)
{
	qint8 *base = reinterpret_cast<qint8*>(buffer);
	QList<double> result;
	for(int i = 0; i < size; ++i)
	{
		result.append(ViPcmConverter::pcmToReal8(base[i]));
	}
	return result;
}

QList<double> ViWaveFormerThread::pcmToReal16(char* buffer, int size)
{
	size /= 2; //char is only 8bit
	qint16 *base = reinterpret_cast<qint16*>(buffer);
	QList<double> result;
	for(int i = 0; i < size; ++i)
	{
		result.append(ViPcmConverter::pcmToReal16(base[i]));
	}
	return result;
}

QList<double> ViWaveFormerThread::pcmToReal32(char* buffer, int size)
{
	size /= 4; //char is only 8bit
	qint32 *base = reinterpret_cast<qint32*>(buffer);
	QList<double> result;
	for(int i = 0; i < size; ++i)
	{
		result.append(ViPcmConverter::pcmToReal32(base[i]));
	}
	return result;
}

ViWaveFormer::ViWaveFormer(ViAudioMetaData *metaData)
	: ViProcessor()
{
	mMetaData = metaData;
}

ViWaveFormer::~ViWaveFormer()
{
	if(mThread != NULL)
	{
		ViObject::disconnect(mThread, SIGNAL(completed(QList<double>)), this, SIGNAL(completed(QList<double>)));
	}
}

void ViWaveFormer::initialize(ViAudioBuffer *buffer)
{
	mThread = new ViWaveFormerThread(buffer, mMetaData, &mSizes);
	ViObject::connectDirect(mThread, SIGNAL(completed(QList<double>)), this, SIGNAL(completed(QList<double>)));
}
