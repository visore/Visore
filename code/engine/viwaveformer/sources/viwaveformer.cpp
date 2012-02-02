#include "viwaveformer.h"

ViWaveFormerThread::ViWaveFormerThread(ViAudioBuffer *buffer)
	: ViProcessorThread(buffer)
{
}

void ViWaveFormerThread::update(int size)
{

}

ViWaveFormer::ViWaveFormer()
	: ViProcessor()
{
}

ViWaveFormer::~ViWaveFormer()
{
	if(mThread != NULL)
	{
		ViObject::disconnect(mThread, SIGNAL(completed(QList<int>)), this, SIGNAL(completed(QList<int>)));
	}
}

void ViWaveFormer::initialize(ViAudioBuffer *buffer)
{
	mThread = new ViWaveFormerThread(buffer);
	ViObject::connect(mThread, SIGNAL(completed(QList<int>)), this, SIGNAL(completed(QList<int>)));
	mThread->start();
}
