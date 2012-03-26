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

void ViWaveFormerThread::addTask(qint64 start, qint64 length)
{
	mMutex.lock();
	mLengths.append(length);
	mMutex.unlock();
	if(!isRunning())
	{
		this->start();
	}
}

void ViWaveFormerThread::run()
{
	mMutex.lock();
	while(!mLengths.isEmpty())
	{
		qint64 length = mLengths.takeFirst();
		ViAudioBufferChunk chunk;
		length = mReadStream->read(&chunk, length * mMetaData->bitDepth() / 8);
		double *result = new double[length];
		length = (this->*pcmToReal)(chunk.data(), result, length);
		emit completed(QSharedPointer<ViWaveFormChunk>(new ViWaveFormChunk(result, length, mMetaData)));
	}
	mMutex.unlock();
}

int ViWaveFormerThread::pcmToReal8(char* buffer, double *result, int size)
{
	return ViPcmConverter::pcmToReal8(buffer, result, size);
}

int ViWaveFormerThread::pcmToReal16(char* buffer, double *result, int size)
{
	return ViPcmConverter::pcmToReal16(buffer, result, size);
}

int ViWaveFormerThread::pcmToReal32(char* buffer, double *result, int size)
{
	return ViPcmConverter::pcmToReal32(buffer, result, size);
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
	ViObject::connectDirect(mThread, SIGNAL(completed(QSharedPointer<ViWaveFormChunk>)), this, SIGNAL(completed(QSharedPointer<ViWaveFormChunk>)));
	ViObject::connectDirect(buffer, SIGNAL(changed(int)), this, SLOT(analyze(int)));
}

void ViWaveFormer::analyze(int length)
{
	mThread->addTask(0, length);
}
