#include "viprocessor.h"

ViProcessorThread::ViProcessorThread(ViAudioBuffer *buffer)
	: QThread()
{
	mBuffer = buffer;
	mReadStream = mBuffer->createReadStream();
	mWriteStream = mBuffer->createWriteStream();
}

void ViProcessorThread::update(int size)
{
	mSizesMutex.lock();
	mSizes.append(size);
	mSizesMutex.unlock();
	if(!isRunning())
	{
		start();
	}
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

void ViProcessor::initialize(ViAudioBuffer *buffer)
{
	if(mThread != NULL)
	{
		ViObject::connect(mThread, SIGNAL(changed()), this, SIGNAL(changed()));
	}
}

void ViProcessor::update(int size)
{
	mThread->update(size);
}
