#include "viexecutor.h"

#define DEFAULT_WINDOW_SIZE 2048

ViExecutor::ViExecutor()
	: QThread()
{
	mWindowSize = DEFAULT_WINDOW_SIZE;
	mNotify = false;
	mWasInitialized = false;
	mInputChunk = NULL;
	mInputSamples = NULL;
	mOutputSamples = NULL;
	mOutputChunk = NULL;
	mReadStream = NULL;
	mWriteStream = NULL;
	QObject::connect(&mTimer, SIGNAL(timeout()), this, SLOT(updateProcessingRate()));
}

ViExecutor::~ViExecutor()
{
	if(mInputChunk != NULL)
	{
		delete mInputChunk;
		mInputChunk = NULL;
	}
	if(mInputSamples != NULL)
	{
		delete mInputSamples;
		mInputSamples = NULL;
	}
	if(mOutputSamples != NULL)
	{
		delete mOutputSamples;
		mOutputSamples = NULL;
	}
	if(mOutputChunk != NULL)
	{
		delete mOutputChunk;
		mOutputChunk = NULL;
	}
}

int ViExecutor::processingRate()
{
	return mProcessingRate;
}

void ViExecutor::setWindowSize(int windowSize)
{
	mWindowSize = windowSize;
}

void ViExecutor::setNotify(bool notify)
{
	mNotify = notify;
}

bool ViExecutor::attach(ViAudio::Mode mode, ViProcessor *processor)
{
	bool result = mProcessors.add(mode, processor);
	return result;
}

bool ViExecutor::detach(ViProcessor *processor)
{
	disconnect(processor);
	return mProcessors.remove(processor);
}

void ViExecutor::setBuffer(ViAudio::Mode mode, ViAudioBuffer *buffer)
{
	if(mode == ViAudio::AudioInput)
	{
		if(mInputChunk == NULL)
		{
			mInputChunk = new ViRawChunk();
		}
		if(mInputSamples == NULL)
		{
			mInputSamples = new ViSampleChunk();
		}
		mReadStream = buffer->createReadStream();
		QObject::disconnect(this, SLOT(execute()));
		QObject::connect(buffer, SIGNAL(changed(int)), this, SLOT(execute()));
	}
	else if(mode == ViAudio::AudioOutput)
	{
		if(mOutputChunk == NULL)
		{
			mOutputChunk = new ViRawChunk();
		}
		if(mOutputSamples == NULL)
		{
			mOutputSamples = new ViSampleChunk();
		}
		mWriteStream = buffer->createWriteStream();
	}
}

int ViExecutor::defaultWindowSize()
{
	return DEFAULT_WINDOW_SIZE;
}

void ViExecutor::setFormat(ViAudioFormat format)
{
	mOutputFormat = format;
	if(mWriteStream != NULL)
	{
		mWriteStream->buffer()->setFormat(mOutputFormat);
		mOutputConverter.setSize(mOutputFormat.sampleSize());
	}
}

void ViExecutor::execute()
{
	if(!mWasInitialized)
	{
		initialize();
	}
	if(!isRunning())
	{
		start();
	}
}

void ViExecutor::initialize()
{
	mWasInitialized = true;
	if(mReadStream != NULL)
	{
		mInputFormat = mReadStream->buffer()->format();
		mInputConverter.setSize(mInputFormat.sampleSize());
		QObject::connect(mReadStream->buffer(), SIGNAL(formatChanged(ViAudioFormat)), this, SLOT(setFormat(ViAudioFormat)), Qt::UniqueConnection);
	}
	if(mWriteStream != NULL)
	{
		mOutputFormat = mInputFormat;
		mWriteStream->buffer()->setFormat(mOutputFormat);
		mOutputConverter.setSize(mOutputFormat.sampleSize());
	}

	QList<ViProcessor*> processors = mProcessors.processors((ViProcessorList::InputObservers | ViProcessorList::InputManipulators | ViProcessorList::DualObservers));
	for(int i = 0; i < processors.size(); ++i)
	{
		processors[i]->setBuffer(mReadStream->buffer(), ViAudio::AudioInput);
		processors[i]->setBuffer(mWriteStream->buffer(), ViAudio::AudioOutput);
	}
	processors = mProcessors.processors(ViProcessorList::OutputObservers);
	for(int i = 0; i < processors.size(); ++i)
	{
		processors[i]->setBuffer(mWriteStream->buffer(), ViAudio::AudioInput);
	}

	processors = mProcessors.processors();
	for(int i = 0; i < processors.size(); ++i)
	{
		processors[i]->setWindowSize(mWindowSize);
		processors[i]->setFormat(mInputFormat);
		processors[i]->initialize();
	}

	if(mNotify)
	{
		for(int i = 0; i < processors.size(); ++i)
		{
			connect(processors[i]);
		}
	}
	else
	{
		for(int i = 0; i < processors.size(); ++i)
		{
			disconnect(processors[i]);
		}
	}

	if(mInputChunk != NULL)
	{
		mInputChunk->resize(mWindowSize * (mInputFormat.sampleSize() / 8));
	}
	if(mInputSamples != NULL)
	{
		mInputSamples->resize(mWindowSize);
	}
	if(mOutputSamples != NULL)
	{
		mOutputSamples->resize(mWindowSize);
	}
	if(mOutputChunk != NULL)
	{
		mOutputChunk->resize(mWindowSize * (mOutputFormat.sampleSize() / 8));
	}

	mProcessedSamples = 0;
	mProcessingRate = 0;
	mRateCounter = 0;
	mTimer.start(1000);
}

void ViExecutor::finalize()
{
	if(mWasInitialized)
	{
		QList<ViProcessor*> processors = mProcessors.processors();
		for(int i = 0; i < processors.size(); ++i)
		{
			processors[i]->finalize();
		}
		mWasInitialized = false;
		mTimer.stop();
	}
}

void ViExecutor::updateProcessingRate()
{
	++mRateCounter;
	mProcessingRate = ((mProcessingRate / mRateCounter) * (mRateCounter - 1)) + ((mProcessedSamples / mInputFormat.channelCount()) / mRateCounter);
	mProcessedSamples = 0;
	emit processingRateChanged(mProcessingRate);
}

void ViExecutor::connect(ViProcessor *processor)
{
	QObject::connect(this, SIGNAL(progressed(short)), processor, SIGNAL(progressed(short)), Qt::UniqueConnection);
	QObject::connect(this, SIGNAL(finished()), processor, SIGNAL(finished()), Qt::UniqueConnection);
}

void ViExecutor::disconnect(ViProcessor *processor)
{
	QObject::disconnect(this, SIGNAL(progressed(short)), processor, SIGNAL(progressed(short)));
	QObject::disconnect(this, SIGNAL(finished()), processor, SIGNAL(finished()));
}

void ViExecutor::start()
{
	QThread::start();
}

void ViExecutor::run()
{
	if(mNotify)
	{
		runNotify();
	}
	else
	{
		runNormal();
	}
}
