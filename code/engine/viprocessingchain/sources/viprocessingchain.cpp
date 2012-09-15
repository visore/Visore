#include "viprocessingchain.h"

#include "viunderrunhandler.h"
#include "viprojecthandler.h"
#include "visectionhandler.h"
#include "viplaybackhandler.h"

ViProcessingChain::ViProcessingChain()
	: QObject()
{
	mInput = NULL;
	mStreamOutput = NULL;
	mFileOutput = NULL;
	mInputBuffer = NULL;
	mOutputBuffer = NULL;
	mMultiExecutor.setNotify(true);
	QObject::connect(&mMultiExecutor, SIGNAL(progressed(short)), this, SIGNAL(changed()));

	mUnderrunHandler = new ViUnderrunHandler(this);
	//mProjectHandler = new ViProjectHandler(this);
	mSectionHandler = new ViSectionHandler(this);
	//mPlaybackHandler = new ViPlaybackHandler(this);
	mHandlers.append(mUnderrunHandler);
	//mHandlers.append(mProjectHandler);
	mHandlers.append(mSectionHandler);
	//mHandlers.append(mPlaybackHandler);
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

	qDeleteAll(mHandlers);
	mHandlers.clear();
}

void ViProcessingChain::startInput()
{
	/*nextBuffer(ViAudio::AudioInput);
	ViAudioBuffer *buffer = allocateBuffer(ViAudio::AudioOutput);
	mMutex.lock();
	mInput->setBuffer(mInputBuffer);
	mMultiExecutor.setBuffer(ViAudio::AudioInput, mInputBuffer);
	mMultiExecutor.setBuffer(ViAudio::AudioOutput, buffer);
	mMultiExecutor.initialize();
	mMutex.unlock();*/
}

void ViProcessingChain::endInput()
{
	/*if(wasSongRunning())
	{
		//deallocateBuffer(ViAudio::AudioOutput);
		//QObject::connect(&mMultiExecutor, SIGNAL(finished()), this, SLOT(finishProcessing()));
	}
	else
	{
		//finishProcessing();
	}
	finishProcessing();*/
	//mInput->setBuffer(allocateBuffer(ViAudio::AudioInput));
}

void ViProcessingChain::startOutput()
{/*LOG("+++: "+QString::number(mOutputBuffers.size()));
	nextBuffer(ViAudio::AudioOutput);
	mMutex.lock();
	mStreamOutput->setBuffer(mOutputBuffer);
	mMutex.unlock();
	mStreamOutput->start();*/
	//emit outputStarted();
}

void ViProcessingChain::endOutput()
{/*LOG("+++1");
	mStreamOutput->stop();
	mMutex.lock();
	delete mOutputBuffer;
	mMutex.unlock();LOG("+++2");*/
}

void ViProcessingChain::finishProcessing()
{
	/*mMutex.lock();
	mMultiExecutor.finalize();
	QObject::disconnect(&mMultiExecutor, SIGNAL(finished()), this, SLOT(finishProcessing()));
	mMutex.unlock();*/
	/*if(wasSongRunning())
	{
		QObject::connect(mStreamOutput, SIGNAL(finished()), this, SLOT(endOutput()));
	}
	else
	{
		endOutput();
	}**/
	//nextBuffer(ViAudio::AudioInput);
	//mMultiExecutor.setBuffer(ViAudio::AudioInput, mInputBuffer);
	//mMultiExecutor.setBuffer(ViAudio::AudioOutput, allocateBuffer(ViAudio::AudioOutput));
	/*if(!mProjectHandler)
	{
		endOutput();
	}
	emit finishedProcessing();*/
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
		//mInput->setBuffer(nextBuffer(ViAudio::AudioInput));
		//mMultiExecutor.setBuffer(ViAudio::AudioInput, mInputBuffer);
		emit inputChanged();
	}
	else if((streamOutput = dynamic_cast<ViStreamOutput*>(transmission)) != NULL)
	{
		mStreamOutput = streamOutput;
		//mStreamOutput->setBuffer(nextBuffer(ViAudio::AudioOutput));
		//mMultiExecutor.setBuffer(ViAudio::AudioOutput, mOutputBuffer);
		emit outputChanged();
	}
	else if((fileOutput = dynamic_cast<ViFileOutput*>(transmission)) != NULL)
	{
		mFileOutput = fileOutput;
		emit outputChanged();
	}

	if(mStreamOutput != NULL && mInput != NULL)
	{
		startInput();
	}
}

void ViProcessingChain::setProject(ViProject *project, ViAudioFormat format)
{
	mProjectHandler->create(project, format);
}

bool ViProcessingChain::attach(ViAudio::Mode mode, ViProcessor *processor)
{
	mMultiExecutor.attach(mode, processor);
	emit attached(processor);
}

bool ViProcessingChain::detach(ViProcessor *processor)
{
	mMultiExecutor.detach(processor);
}

bool ViProcessingChain::isSongRunning()
{
	return mSectionHandler->isSongRunning();
}

bool ViProcessingChain::wasSongRunning()
{
	return mSectionHandler->wasSongRunning();
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
	mMutex.lock();
	if(mode == ViAudio::AudioInput)
	{
		mInputBuffers.enqueue(buffer);
	}
	else if(mode == ViAudio::AudioOutput)
	{
		mOutputBuffers.enqueue(buffer);
	}
	mMutex.unlock();
	return buffer;
}

void ViProcessingChain::deallocateBuffer(ViAudio::Mode mode)
{
	ViAudioBuffer *buffer = NULL;
	mMutex.lock();
	if(mode == ViAudio::AudioInput)
	{
		buffer = mInputBuffers.dequeue();
	}
	else if(mode == ViAudio::AudioOutput)
	{
		buffer = mOutputBuffers.dequeue();
	}
	if(buffer != NULL)
	{
		delete buffer;
		buffer = NULL;
	}
	mMutex.unlock();
}

ViAudioBuffer* ViProcessingChain::nextBuffer(ViAudio::Mode mode)
{
	mMutex.lock();
	if(mode == ViAudio::AudioInput)
	{
		if(mInputBuffer != NULL)
		{
			delete mInputBuffer;
		}
		if(mInputBuffers.isEmpty())
		{
			mMutex.unlock();
			allocateBuffer(ViAudio::AudioInput);
			mMutex.lock();
		}
		mInputBuffer = mInputBuffers.dequeue();
		mMutex.unlock();
		return mInputBuffer;
	}
	else if(mode == ViAudio::AudioOutput)
	{
		if(mOutputBuffer != NULL)
		{
			delete mOutputBuffer;
		}
		if(mOutputBuffers.isEmpty())
		{
			mMutex.unlock();
			allocateBuffer(ViAudio::AudioOutput);
			mMutex.lock();
		}
		mOutputBuffer = mOutputBuffers.dequeue();
		mMutex.unlock();
		return mOutputBuffer;
	}
}
