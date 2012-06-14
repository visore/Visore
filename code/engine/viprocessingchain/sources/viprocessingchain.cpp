#include "viprocessingchain.h"

#define DEFAULT_SONG_LENGTH 240000

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

void ViProcessingChain::changeInput(ViAudioPosition position)
{
	if(!mEndDetected)
	{
		mEndDetected = true;
		QObject::connect(&mMultiExecutor, SIGNAL(finished()), this, SLOT(finish()));
		mInput->setBuffer(allocateBuffer(ViAudio::AudioInput));
	}
}

void ViProcessingChain::finish()
{
	mMultiExecutor.finalize();
	QObject::disconnect(&mMultiExecutor, SIGNAL(finished()), this, SLOT(finish()));
	//Save to file here
	nextBuffer();
	mEndDetected = false;
	mMultiExecutor.setBuffer(ViAudio::AudioInput, mInputBuffer);
}

void ViProcessingChain::handleUnderrun()
{
	mSecondsPassed = 0;
	mSecondsNeeded = 0;
	QObject::connect(&mMultiExecutor, SIGNAL(processingRateChanged(qreal)), this, SLOT(updateBuffering(qreal)));
}

void ViProcessingChain::updateBuffering(qreal processingRate)
{
	if(mSecondsNeeded == 0)
	{
		qreal ratio = 1 - (processingRate / mOutputBuffer->format().sampleRate());
		ratio *= 1 + ratio;
		qint64 bytesNeeded = ViAudioPosition::convertPosition(DEFAULT_SONG_LENGTH, ViAudioPosition::Milliseconds, ViAudioPosition::Bytes, mOutputBuffer->format());
		bytesNeeded -= mOutput->position().position(ViAudioPosition::Bytes);
		bytesNeeded *= ratio;
		mSecondsNeeded = ViAudioPosition::convertPosition(bytesNeeded, ViAudioPosition::Bytes, ViAudioPosition::Seconds, mOutputBuffer->format());
		LOG("Buffering started (processing rate: " + QString::number(mMultiExecutor.processingRate(), 'f', 1) + "Hz, buffer needed: " + QString::number(mSecondsNeeded) + "s)");
	}
	if(mSecondsNeeded < 0)
	{
		return;
	}

	short progress = mSecondsPassed / (mSecondsNeeded / 100.0);
	++mSecondsPassed;
	if(progress >= 100)
	{
		QObject::disconnect(&mMultiExecutor, SIGNAL(processingRateChanged(qreal)), this, SLOT(updateBuffering(qreal)));
		mOutput->start();
	}
	emit buffering(progress);
}

void ViProcessingChain::setWindowSize(int windowSize)
{
	mMultiExecutor.setWindowSize(windowSize);
}

void ViProcessingChain::setTransmission(ViAudioTransmission *transmission)
{
	ViAudioInput *input;
	ViStreamOutput *output;
	if((input = dynamic_cast<ViAudioInput*>(transmission)) != NULL)
	{
		mInput = input;
		allocateBuffer(ViAudio::AudioInput);
		nextBuffer();
		mInput->setBuffer(mInputBuffer);
		mMultiExecutor.setBuffer(ViAudio::AudioInput, mInputBuffer);
	}
	else if((output = dynamic_cast<ViStreamOutput*>(transmission)) != NULL)
	{
		mOutput = output;
		QObject::connect(mOutput, SIGNAL(underrun()), this, SLOT(handleUnderrun()));
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

ViAudioBuffer* ViProcessingChain::allocateBuffer(ViAudio::Mode mode)
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
	return buffer;
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
