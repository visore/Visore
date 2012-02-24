#include "viprocessor.h"

ViProcessorThread::ViProcessorThread(ViAudioBuffer *buffer, QList<int> *sizes)
	: QThread()
{
	mBuffer = buffer;
	mSizes = sizes;
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
	/*mSizes.append(size);
	if(!mThread->isRunning())
	{
		//mThread->run();
		mThread->start();
	}*/
}
