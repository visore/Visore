#include "viwaveformer.h"

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
		mReadStream->read(&chunk, size);

		char *base = chunk.data();
		QList<double> result;

		for(int i = 0; i < size; ++i)
		{
			result.append((this->*pcmToReal)(base[i]));
		}

		emit completed(result);
	}
}

double ViWaveFormerThread::pcmToReal8(char character)
{
	return ViPcmConverter::pcmToReal8((qint8) character);
}

double ViWaveFormerThread::pcmToReal16(char character)
{
	return ViPcmConverter::pcmToReal16((qint16) character);
}

double ViWaveFormerThread::pcmToReal32(char character)
{
	return ViPcmConverter::pcmToReal32((qint32) character);
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
	ViObject::connect(mThread, SIGNAL(completed(QList<double>)), this, SIGNAL(completed(QList<double>)));
}
