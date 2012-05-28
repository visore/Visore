#include "viprocessingchain.h"

ViProcessingChain::ViProcessingChain()
	: QObject()
{
	mInput = NULL;
	mOutput = NULL;
	mInputBuffer = NULL;
	mOutputBuffer = NULL;
	mMultiExecutor.setNotify(true);
	QObject::connect(&mMultiExecutor, SIGNAL(progressed(short)), this, SIGNAL(changed()));
}

ViProcessingChain::~ViProcessingChain()
{
	if(mInputBuffer != NULL)
	{
		delete mInputBuffer;
		mInputBuffer = NULL;
	}
	qDeleteAll(mInputBuffers);
	mInputBuffers.clear();
	if(mOutputBuffer != NULL)
	{
		delete mOutputBuffer;
		mOutputBuffer = NULL;
	}
}

void ViProcessingChain::changeInput()
{
	QObject::connect(&mMultiExecutor, SIGNAL(finished()), this, SLOT(finish()));
	allocateBuffer(ViAudioConnection::Input);
	mInput->setBuffer(mInputBuffer);
}

void ViProcessingChain::finish()
{
	QObject::disconnect(&mMultiExecutor, SIGNAL(finished()), this, SLOT(finish()));
	//Save to file here
	nextBuffer();
}

void ViProcessingChain::setWindowSize(int windowSize)
{
	mMultiExecutor.setWindowSize(windowSize);
}

void ViProcessingChain::setTransmission(ViAudioTransmission *transmission)
{
	ViAudioInput *input;
	ViAudioOutput *output;
	if((input = dynamic_cast<ViAudioInput*>(transmission)) != NULL)
	{
		mInput = input;
		allocateBuffer(ViAudioConnection::Input);
		nextBuffer();
		mInput->setBuffer(mInputBuffer);
		mMultiExecutor.setBuffer(ViAudioConnection::Input, mInputBuffer);
	}
	else if((output = dynamic_cast<ViAudioOutput*>(transmission)) != NULL)
	{
		mOutput = output;
		allocateBuffer(ViAudioConnection::Output);
		mOutput->setBuffer(mOutputBuffer);
		mMultiExecutor.setBuffer(ViAudioConnection::Output, mOutputBuffer);
	}
}

bool ViProcessingChain::attach(ViAudioConnection::Direction direction, ViProcessor *processor)
{
	mMultiExecutor.attach(direction, processor);
}

bool ViProcessingChain::detach(ViProcessor *processor)
{
	mMultiExecutor.detach(processor);
}

ViAudioBuffer* ViProcessingChain::buffer(ViAudioConnection::Direction direction)
{
	if(direction == ViAudioConnection::Input)
	{
		return mInputBuffer;
	}
	else if(direction == ViAudioConnection::Output)
	{
		return mOutputBuffer;
	}
}

void ViProcessingChain::allocateBuffer(ViAudioConnection::Direction direction)
{
	ViAudioBuffer *buffer = new ViAudioBuffer();
	if(direction == ViAudioConnection::Input)
	{
		mInputBuffers.enqueue(buffer);
	}
	else if(direction == ViAudioConnection::Output)
	{
		if(mOutputBuffer != NULL)
		{
			delete mOutputBuffer;
		}
		mOutputBuffer = buffer;
	}
}

void ViProcessingChain::nextBuffer()
{
	if(mInputBuffer != NULL)
	{
		delete mInputBuffer;
	}
	if(mInputBuffers.isEmpty())
	{
		mInputBuffer = NULL;
	}
	else
	{
		mInputBuffer = mInputBuffers.dequeue();
	}
	if(mOutputBuffer != NULL)
	{
		mOutputBuffer->clear();
	}
}
