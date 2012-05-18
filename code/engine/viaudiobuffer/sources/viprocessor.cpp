#include "viprocessor.h"

ViProcessorThread::ViProcessorThread(ViAudioBuffer *buffer, QIODevice::OpenMode mode)
	: QThread()
{
	mBuffer = buffer;
	if(mode == QIODevice::ReadOnly || mode == QIODevice::ReadWrite)
	{
		mReadStream = mBuffer->createReadStream();
	}
	else
	{
		mReadStream = NULL;
	}
	if(mode == QIODevice::WriteOnly || mode == QIODevice::ReadWrite)
	{
		mWriteStream = mBuffer->createWriteStream();
	}
	else
	{
		mWriteStream = NULL;
	}
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
	QObject::connect(mThread, SIGNAL(changed()), this, SIGNAL(changed()));
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
