#include "viprocessingchain.h"
#include "viaudiobuffermanager.h"
#include "vipcmconverter.h"
#include "viqtconnection.h"

#define BLOCK_SIZE 2048

ViProcessingThread::ViProcessingThread()
	: QThread()
{
	mRawChunk.resize(BLOCK_SIZE);
}

ViProcessingThread::~ViProcessingThread()
{
	qDeleteAll(mRawInputProcessors);
	mRawInputProcessors.clear();
	qDeleteAll(mRealInputProcessors);
	mRealInputProcessors.clear();

	qDeleteAll(mRawOutputProcessors);
	mRawOutputProcessors.clear();
	qDeleteAll(mRealOutputProcessors);
	mRealOutputProcessors.clear();
}

void ViProcessingThread::setStream(ViAudioConnection::Direction direction, ViAudioBufferStream *stream)
{
	if(direction == ViAudioConnection::Input)
	{
		mReadStream = stream;
	}
	else
	{
		mWriteStream = stream;
	}
}

void ViProcessingThread::setSampleSize(ViAudioConnection::Direction direction, int sampleSize)
{
	if(direction == ViAudioConnection::Input)
	{
		mRealChunk.resize(BLOCK_SIZE / (sampleSize / 8));
		if(sampleSize == 8)
		{
			pcmToReal = &ViPcmConverter<double>::pcmToReal8;
		}
		else if(sampleSize == 16)
		{
			pcmToReal = &ViPcmConverter<double>::pcmToReal16;
		}
		else if(sampleSize == 24)
		{
			pcmToReal = &ViPcmConverter<double>::pcmToReal24;
		}
		else if(sampleSize == 32)
		{
			pcmToReal = &ViPcmConverter<double>::pcmToReal32;
		}
	}
	else
	{

	}
}

void ViProcessingThread::attach(ViAudioConnection::Direction direction, ViProcessor *processor)
{
	ViRawProcessor *rawProcessor;
	ViRealProcessor *realProcessor;
	if((rawProcessor = dynamic_cast<ViRawProcessor*>(processor)) != NULL)
	{
		if(direction == ViAudioConnection::Input)
		{
			mRawInputProcessors.append(rawProcessor);
		}
		else
		{
			mRawOutputProcessors.append(rawProcessor);
		}
		
	}
	else if((realProcessor = dynamic_cast<ViRealProcessor*>(processor)) != NULL)
	{
		if(direction == ViAudioConnection::Input)
		{
			mRealInputProcessors.append(realProcessor);
		}
		else
		{
			mRealOutputProcessors.append(realProcessor);
		}
	}
}

void ViProcessingThread::run()
{
	do
	{
		mRawChunk.setSize(mReadStream->read(&mRawChunk));
		if(mRawChunk.size() > 0)
		{
			for(int i = 0; i < mRawInputProcessors.size(); ++i)
			{
				mRawInputProcessors[i]->setData(&mRawChunk);
				mRawInputProcessors[i]->run();
			}
			mRealChunk.setSize(pcmToReal(mRawChunk.data(), mRealChunk.data(), mRawChunk.size()));
			for(int i = 0; i < mRealInputProcessors.size(); ++i)
			{
				mRealInputProcessors[i]->setData(&mRealChunk);
				mRealInputProcessors[i]->run();
			}


			for(int i = 0; i < mRawOutputProcessors.size(); ++i)
			{
				mRawOutputProcessors[i]->setData(&mRawChunk);
				mRawOutputProcessors[i]->run();
			}
			mRealChunk.setSize(pcmToReal(mRawChunk.data(), mRealChunk.data(), mRawChunk.size()));
			for(int i = 0; i < mRealOutputProcessors.size(); ++i)
			{
				mRealOutputProcessors[i]->setData(&mRealChunk);
				mRealOutputProcessors[i]->run();
			}


			mWriteStream->write(&mRawChunk);
		}
	}
	while(mRawChunk.size() > 0);
}

ViProcessingChain::ViProcessingChain()
{
	mConnection = new ViQtConnection();
	mInputBuffer = NULL;
	mOutputBuffer = NULL;
}

ViProcessingChain::~ViProcessingChain()
{
	ViAudioBufferManager::deallocateAll();
	delete mConnection;
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

ViAudioBuffer* ViProcessingChain::buffer(ViAudioConnection::Direction direction, ViAudioConnection::Type type)
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

void ViProcessingChain::attach(ViAudioConnection::Direction direction, ViProcessor *processor)
{
	mThread.attach(direction, processor);
}

void ViProcessingChain::setInput(ViAudioFormat format, QString filePath)
{
	changeInputBuffer(allocateBuffer(ViAudioConnection::Input));
	mInput = mConnection->fileInput(format, mInputBuffer, filePath);
	mThread.setSampleSize(ViAudioConnection::Input, format.sampleSize());
}

void ViProcessingChain::setInput(ViAudioFormat format, QAudioDeviceInfo device)
{
	changeInputBuffer(allocateBuffer(ViAudioConnection::Input));
	mInput = mConnection->streamInput(format, mInputBuffer, device);
	mThread.setSampleSize(ViAudioConnection::Input, format.sampleSize());
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
