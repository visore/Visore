#include "viwaveformer.h"

ViWaveFormerThread::ViWaveFormerThread(ViAudioBuffer *buffer, ViAudioMetaData *metaData, QList<int> *sizes)
	: ViProcessorThread(buffer, sizes)
{
	qRegisterMetaType<ViWaveFormChunk>("ViWaveFormChunk"); //Ensure ViWaveFormChunk works with signal and slots
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
		double *result = new double[size];
		size = (this->*pcmToReal)(chunk.data(), result, size);
		emit completed(new ViWaveFormChunk(result, size, mMetaData));
	}
}

int ViWaveFormerThread::pcmToReal8(char* buffer, double *result, int size)
{
	qint8 *base = reinterpret_cast<qint8*>(buffer);
	for(int i = 0; i < size; ++i)
	{
		result[i] = ViPcmConverter::pcmToReal8(base[i]);
	}
	return size;
}

int ViWaveFormerThread::pcmToReal16(char* buffer, double *result, int size)
{
	size /= 2; //char is only 8bit
	qint16 *base = reinterpret_cast<qint16*>(buffer);
	for(int i = 0; i < size; ++i)
	{
		result[i] = ViPcmConverter::pcmToReal16(base[i]);
	}
	return size;
}

int ViWaveFormerThread::pcmToReal32(char* buffer, double *result, int size)
{
	size /= 4; //char is only 8bit
	qint32 *base = reinterpret_cast<qint32*>(buffer);
	for(int i = 0; i < size; ++i)
	{
		result[i] = ViPcmConverter::pcmToReal32(base[i]);
	}
	return size;
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
		ViObject::disconnect(mThread, SIGNAL(completed(ViWaveFormChunk*)), this, SIGNAL(completed(ViWaveFormChunk*)));
	}
}

void ViWaveFormer::initialize(ViAudioBuffer *buffer)
{
	mThread = new ViWaveFormerThread(buffer, mMetaData, &mSizes);
	ViObject::connectDirect(mThread, SIGNAL(completed(ViWaveFormChunk*)), this, SIGNAL(completed(ViWaveFormChunk*)));
}
