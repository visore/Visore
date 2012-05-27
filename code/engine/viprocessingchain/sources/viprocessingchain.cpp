#include "viprocessingchain.h"
#include "viaudiobuffermanager.h"
#include "vipcmconverter.h"
#include "viqtconnection.h"

ViProcessingChain::ViProcessingChain()
{
	mConnection = new ViQtConnection();
	mInputBuffer = NULL;
	mOutputBuffer = NULL;
	QObject::connect(&mThread, SIGNAL(progressed(short)), this, SIGNAL(progressed(short)));
	QObject::connect(&mThread, SIGNAL(finished()), this, SIGNAL(finished()));
}

ViProcessingChain::~ViProcessingChain()
{
	ViAudioBufferManager::deallocateAll();
	delete mConnection;
}

void ViProcessingChain::setWindowSize(int windowSize)
{
	mThread.setWindowSize(windowSize);
}

void ViProcessingChain::process()
{
	if(!mThread.isRunning())
	{
		mThread.start();
	}
}


void ViProcessingChain::start()
{
	for(int i = 0; i < mOutputs.size(); ++i)
	{
		mOutputs[i]->start();
	}
	mInput->start();
}

ViAudioBuffer* ViProcessingChain::buffer(ViAudioConnection::Direction direction)
{
	if(direction == ViAudioConnection::Input)
	{
		return mInputBuffer;
	}
	else
	{
		return mOutputBuffer;
	}
}

bool ViProcessingChain::attach(ViAudioConnection::Direction direction, ViProcessor *processor)
{
	return mThread.attach(direction, processor);
}

void ViProcessingChain::setInput(ViAudioFormat format, QString filePath)
{
	changeInputBuffer(allocateBuffer(ViAudioConnection::Input));
	mInput = mConnection->fileInput(format, mInputBuffer, filePath);
}

void ViProcessingChain::setInput(ViAudioFormat format, QAudioDeviceInfo device)
{
	changeInputBuffer(allocateBuffer(ViAudioConnection::Input));
	mInput = mConnection->streamInput(format, mInputBuffer, device);
}

void ViProcessingChain::addOutput(ViAudioFormat format, QString filePath)
{
	changeOutputBuffer(allocateBuffer(ViAudioConnection::Output));
	mOutputs.append(mConnection->fileOutput(format, mOutputBuffer, filePath));
}

void ViProcessingChain::addOutput(ViAudioFormat format, QAudioDeviceInfo device)
{
	changeOutputBuffer(allocateBuffer(ViAudioConnection::Output));
	mOutputs.append(mConnection->streamOutput(format, mOutputBuffer, device));
}

void ViProcessingChain::changeInputBuffer(ViAudioBuffer *buffer)
{
	if(mInputBuffer != NULL)
	{
		QObject::disconnect(mInputBuffer, SIGNAL(changed(int)), this, SLOT(process()));
		QObject::disconnect(mInputBuffer, SIGNAL(changed(int)), this, SLOT(inputChanged()));
	}
	mInputBuffer = buffer;
	mInputBuffer->setFormat(ViAudioFormat::defaultFormat()); // TODO: change to format of input
	if(mInputBuffer != NULL)
	{
		QObject::connect(mInputBuffer, SIGNAL(changed(int)), this, SLOT(process()));
		QObject::connect(mInputBuffer, SIGNAL(changed(int)), this, SIGNAL(inputChanged()));
		mThread.setStream(ViAudioConnection::Input, mInputBuffer->createReadStream());
	}
}

void ViProcessingChain::changeOutputBuffer(ViAudioBuffer *buffer)
{
	if(mOutputBuffer != NULL)
	{
		QObject::disconnect(mOutputBuffer, SIGNAL(changed(int)), this, SIGNAL(outputChanged()));
	}
	mOutputBuffer = buffer;
	mOutputBuffer->setFormat(ViAudioFormat::defaultFormat()); // TODO: change to input format by default
	if(mOutputBuffer != NULL)
	{
		QObject::connect(mOutputBuffer, SIGNAL(changed(int)), this, SIGNAL(outputChanged()));
		mThread.setStream(ViAudioConnection::Output, mOutputBuffer->createWriteStream());
	}
}

ViAudioBuffer* ViProcessingChain::allocateBuffer(ViAudioConnection::Direction direction)
{
	ViAudioBuffer *buffer = ViAudioBufferManager::allocate();
	if(direction == ViAudioConnection::Input)
	{
		mInputBuffers.enqueue(buffer);
	}
	else if(direction == ViAudioConnection::Output)
	{
		mOutputBuffer = buffer;
	}
	return buffer;
}

void ViProcessingChain::nextBuffer()
{
	ViAudioBufferManager::deallocate(mOutputBuffer);
	ViAudioBufferManager::deallocate(mInputBuffers.dequeue());
	if(mInputBuffers.isEmpty())
	{
		changeOutputBuffer(NULL);
		changeInputBuffer(NULL);
	}
	else
	{
		changeOutputBuffer(allocateBuffer(ViAudioConnection::Output));
		changeInputBuffer(mInputBuffers.first());
	}
}
