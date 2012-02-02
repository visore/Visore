#include "viprocessor.h"

ViProcessorThread::ViProcessorThread(ViAudioBuffer *buffer)
	: QThread()
{
	mBuffer = buffer;
	mReadStream = mBuffer->createReadStream();
	mWriteStream = mBuffer->createWriteStream();
}

ViProcessor::ViProcessor()
	: QObject()
{
	mThread = NULL;
	mId = ViProcessorManager::nextId();
}

ViProcessor::~ViProcessor()
{
	if(mThread != NULL)
	{
		mThread->quit();
		delete mThread;
		mThread = NULL;
	}
}

int ViProcessor::id()
{
	return mId;
}

void ViProcessor::update(int size)
{
	mThread->update(size);
}
