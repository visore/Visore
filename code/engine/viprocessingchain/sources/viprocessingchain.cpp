#include "viprocessingchain.h"
#include "viaudiocodec.h"

#define DEFAULT_SONG_LENGTH 240000
#define MINIMUM_SONG_LENGTH 1500

ViProcessingChain::ViProcessingChain()
	: QObject()
{
	mInput = NULL;
	mStreamOutput = NULL;
	mFileOutput = NULL;
	mInputBuffer = NULL;
	mOutputBuffer = NULL;
	mProject = NULL;
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
	qDeleteAll(mOutputBuffers);
	mOutputBuffers.clear();

	if(mProject != NULL)
	{
		delete mProject;
		mProject = NULL;
	}
}

void ViProcessingChain::changeInput(ViAudioPosition position)
{
	QObject::connect(&mMultiExecutor, SIGNAL(finished()), this, SLOT(finalize()));
	mInput->setBuffer(allocateBuffer(ViAudio::AudioInput));
}

void ViProcessingChain::startInput(ViAudioPosition position)
{
	allocateBuffer(ViAudio::AudioInput);
	allocateBuffer(ViAudio::AudioOutput);
	nextBuffer(ViAudio::AudioInput);
	nextBuffer(ViAudio::AudioOutput);
	mInput->setBuffer(mInputBuffer);
	mMultiExecutor.setBuffer(ViAudio::AudioInput, mInputBuffer);
	mMultiExecutor.setBuffer(ViAudio::AudioOutput, mOutputBuffer);
	mMultiExecutor.initialize();
}

void ViProcessingChain::finalize()
{
	mMultiExecutor.finalize();
	QObject::disconnect(&mMultiExecutor, SIGNAL(finished()), this, SLOT(finalize()));
	nextBuffer(ViAudio::AudioInput);
	mMultiExecutor.setBuffer(ViAudio::AudioInput, mInputBuffer);
	mMultiExecutor.setBuffer(ViAudio::AudioOutput, allocateBuffer(ViAudio::AudioOutput));
	qreal songLength = ViAudioPosition::convertPosition(mOutputBuffer->size(), ViAudioPosition::Samples, ViAudioPosition::Milliseconds, mOutputBuffer->format());
	if(mProject != NULL && songLength >= MINIMUM_SONG_LENGTH)
	{
		QObject::connect(mFileOutput, SIGNAL(finished()), this, SLOT(finish()));
		mFileOutput->setBuffer(mOutputBuffer);
		mFileOutput->setFile(mProject->originalPath() + QDir::separator() + mProject->nextSongName() + "." + mFileOutput->format().codec()->extensions()[0]);
		mFileOutput->start();
	}
	else
	{
		nextBuffer(ViAudio::AudioOutput);
	}
}

void ViProcessingChain::finish()
{
	mProject->save();
	QObject::disconnect(mFileOutput, SIGNAL(finished()), this, SLOT(finish()));
	nextBuffer(ViAudio::AudioOutput);
	mMultiExecutor.initialize();
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
		bytesNeeded -= mStreamOutput->position().position(ViAudioPosition::Bytes);
		bytesNeeded *= ratio;
		mSecondsNeeded = ViAudioPosition::convertPosition(bytesNeeded, ViAudioPosition::Bytes, ViAudioPosition::Seconds, mOutputBuffer->format());
		LOG("Buffering started (processing rate: " + QString::number(mMultiExecutor.processingRate(), 'f', 1) + "Hz, buffer needed: " + QString::number(mSecondsNeeded) + "s)");
	}
	if(mSecondsNeeded < 0)
	{
		mSecondsNeeded = 5;
	}

	short progress = mSecondsPassed / (mSecondsNeeded / 100.0);
	++mSecondsPassed;
	if(progress >= 100)
	{
		QObject::disconnect(&mMultiExecutor, SIGNAL(processingRateChanged(qreal)), this, SLOT(updateBuffering(qreal)));
		mStreamOutput->start();
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
	ViStreamOutput *streamOutput;
	ViFileOutput *fileOutput;
	if((input = dynamic_cast<ViAudioInput*>(transmission)) != NULL)
	{
		mInput = input;
		allocateBuffer(ViAudio::AudioInput);
		nextBuffer(ViAudio::AudioInput);
		mInput->setBuffer(mInputBuffer);
		mMultiExecutor.setBuffer(ViAudio::AudioInput, mInputBuffer);
	}
	else if((streamOutput = dynamic_cast<ViStreamOutput*>(transmission)) != NULL)
	{
		mStreamOutput = streamOutput;
		QObject::connect(mStreamOutput, SIGNAL(underrun()), this, SLOT(handleUnderrun()));
		allocateBuffer(ViAudio::AudioOutput);
		nextBuffer(ViAudio::AudioOutput);
		mStreamOutput->setBuffer(mOutputBuffer);
		mMultiExecutor.setBuffer(ViAudio::AudioOutput, mOutputBuffer);
	}
	else if((fileOutput = dynamic_cast<ViFileOutput*>(transmission)) != NULL)
	{
		mFileOutput = fileOutput;
	}
}

void ViProcessingChain::setProject(QString filePath, ViAudioFormat format)
{
	mFileOutput->setFormat(format);
	if(mProject != NULL)
	{
		delete mProject;
	}
	mProject = new ViProject(filePath);
	mProject->save();
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
		mOutputBuffers.enqueue(buffer);
	}
	return buffer;
}

void ViProcessingChain::nextBuffer(ViAudio::Mode mode)
{
	if(mode == ViAudio::AudioInput)
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
	}
	else if(mode == ViAudio::AudioOutput)
	{
		if(mOutputBuffer != NULL)
		{
			delete mOutputBuffer;
		}
		if(mOutputBuffers.isEmpty())
		{
			mOutputBuffer = NULL;
		}
		else
		{
			mOutputBuffer = mOutputBuffers.dequeue();
		}
	}
}
