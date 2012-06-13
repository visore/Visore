#include "viprocessingchain.h"

ViProcessingChain::ViProcessingChain()
	: QObject()
{
	mEndDetected = false;
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
	if(!mEndDetected)
	{
		mEndDetected = true;
		cout<<"+++++++++++++++++ END +++++++++++++++++++++++"<<endl;
		QObject::connect(&mMultiExecutor, SIGNAL(finished()), this, SLOT(finish()));
		allocateBuffer(ViAudio::AudioInput);
		mInput->setBuffer(mInputBuffer);
	}
}

void ViProcessingChain::finish()
{
	QObject::disconnect(&mMultiExecutor, SIGNAL(finished()), this, SLOT(finish()));
	//Save to file here
	nextBuffer();
	mEndDetected = false;
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
		allocateBuffer(ViAudio::AudioInput);
		nextBuffer();
		mInput->setBuffer(mInputBuffer);
		mMultiExecutor.setBuffer(ViAudio::AudioInput, mInputBuffer);
	}
	else if((output = dynamic_cast<ViAudioOutput*>(transmission)) != NULL)
	{
		mOutput = output;
		allocateBuffer(ViAudio::AudioOutput);
		mOutput->setBuffer(mOutputBuffer);
		mMultiExecutor.setBuffer(ViAudio::AudioOutput, mOutputBuffer);
	}
}

bool ViProcessingChain::attach(ViAudio::Mode mode, ViProcessor *processor)
{
	mMultiExecutor.attach(mode, processor);
}

bool ViProcessingChain::detach(ViProcessor *processor)
{
	mMultiExecutor.detach(processor);
}

ViAudioBuffer* ViProcessingChain::buffer(ViAudio::Mode mode)
{
	if(mode == ViAudio::AudioInput)
	{
		return mInputBuffer;
	}
	else if(mode == ViAudio::AudioOutput)
	{
		return mOutputBuffer;
	}
}

void ViProcessingChain::allocateBuffer(ViAudio::Mode mode)
{
	ViAudioBuffer *buffer = new ViAudioBuffer();
	if(mode == ViAudio::AudioInput)
	{
		mInputBuffers.enqueue(buffer);
	}
	else if(mode == ViAudio::AudioOutput)
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
