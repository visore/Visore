#include "viwaveformer.h"
#include <iostream>
using namespace std;
ViWaveFormerThread::ViWaveFormerThread(ViAudioBuffer *buffer, QList<int> *sizes)
	: ViProcessorThread(buffer, sizes)
{
}

void ViWaveFormerThread::run()
{
	while(!mSizes->isEmpty())
	{
		int size = mSizes->first();
		mSizes->removeFirst();
		ViAudioBufferChunk chunk;
		size = mReadStream->read(&chunk, size);

		char *base = chunk.data();
		QList<double> result;

		for(int i = 0; i < size; ++i)
		{
			result.append(ViPcmConverter::pcmToReal((qint16) base[i]));
		}

		emit completed(result);
	}
}

ViWaveFormer::ViWaveFormer()
	: ViProcessor()
{
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
	mThread = new ViWaveFormerThread(buffer, &mSizes);
	ViObject::connect(mThread, SIGNAL(completed(QList<double>)), this, SIGNAL(completed(QList<double>)));
}
