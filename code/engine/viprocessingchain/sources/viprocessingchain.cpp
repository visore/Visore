#include "viprocessingchain.h"
#include "viaudiobuffermanager.h"
#include "vipcmconverter.h"
#include "viqtconnection.h"

#define DEFAULT_WINDOW_SIZE 2048

ViProcessingThread::ViProcessingThread()
	: QThread()
{
	mWindowSize = DEFAULT_WINDOW_SIZE;
	mReadStream = NULL;
	mWriteStream = NULL;
}

void ViProcessingThread::setWindowSize(int windowSize)
{
	mWindowSize = windowSize;
	updateChunks();
	updateProcessors();
}

void ViProcessingThread::setStream(ViAudioConnection::Direction direction, ViAudioBufferStream *stream)
{
	int sampleSize;
	if(direction == ViAudioConnection::Input)
	{
		mReadStream = stream;
		sampleSize = mReadStream->buffer()->format().sampleSize();
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
		mWriteStream = stream;
		sampleSize = mWriteStream->buffer()->format().sampleSize();
		if(sampleSize == 8)
		{
			realToPcm = &ViPcmConverter<double>::realToPcm8;
		}
		else if(sampleSize == 16)
		{
			realToPcm = &ViPcmConverter<double>::realToPcm16;
		}
		else if(sampleSize == 24)
		{
			realToPcm = &ViPcmConverter<double>::realToPcm24;
		}
		else if(sampleSize == 32)
		{
			realToPcm = &ViPcmConverter<double>::realToPcm32;
		}
	}
	updateChunks();
	updateProcessors();
}

bool ViProcessingThread::attach(ViAudioConnection::Direction direction, ViProcessor *processor)
{
	if(mProcessors.add(direction, processor))
	{
		updateProcessors();
	}
	return false;
}

void ViProcessingThread::updateProcessors()
{
	if(mReadStream != NULL)
	{
		ViAudioFormat format = mReadStream->buffer()->format();
		QList<ViProcessor*> processors = mProcessors.processors();
		for(int i = 0; i < processors.size(); ++i)
		{
			processors[i]->setWindowSize(mWindowSize);
			processors[i]->setFormat(format);
		}
	}
}

void ViProcessingThread::updateChunks()
{
	if(mReadStream != NULL)
	{
		mInputChunk.resize(mWindowSize * (mReadStream->buffer()->format().sampleSize() / 8));
	}
	mRealChunk.resize(mWindowSize);
	if(mWriteStream != NULL)
	{
		mOutputChunk.resize(mWindowSize * (mWriteStream->buffer()->format().sampleSize() / 8));
	}
}

void ViProcessingThread::run()
{
	do
	{
		mInputChunk.setSize(mReadStream->read(&mInputChunk));
		if(mInputChunk.size() > 0)
		{
			mRealChunk.setSize(pcmToReal(mInputChunk.data(), mRealChunk.data(), mInputChunk.size()));
			mProcessors.observeInput(&mRealChunk);
			mProcessors.manipulateInput(&mRealChunk);
			mProcessors.observeOutput(&mRealChunk);
			mOutputChunk.setSize(realToPcm(mRealChunk.data(), mOutputChunk.data(), mRealChunk.size()));
			mWriteStream->write(&mOutputChunk);
		}
	}
	while(mInputChunk.size() > 0);
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
