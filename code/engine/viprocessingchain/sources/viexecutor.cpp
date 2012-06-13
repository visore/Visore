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
	else
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
	}
	if(mWriteStream != NULL)
	{
		mOutputFormat = mWriteStream->buffer()->format();
		mOutputConverter.setSize(mOutputFormat.sampleSize());
	}

	QList<ViProcessor*> processors = mProcessors.all();
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
}

void ViExecutor::finalize()
{
	if(mWasInitialized)
	{
		QList<ViProcessor*> processors = mProcessors.all();
		for(int i = 0; i < processors.size(); ++i)
		{
			processors[i]->finalize();
		}
		mWasInitialized = false;
	}
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
