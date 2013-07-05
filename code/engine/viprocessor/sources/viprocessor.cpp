#include <viprocessor.h>
#include <vipowercalculator.h>
#include <vinoisedetector.h>

void ViProcessorThread::setProcessor(ViProcessor *processor)
{
	mProcessor = processor;
}

void ViProcessorThread::run()
{
	mProcessor->executeThread();
}

ViProcessor::ViProcessor(ViProcessor::ChannelMode mode)
    : QRunnable()
{
    setAutoDelete(false);
	mThreadPool = new QThreadPool();
	mThreadPool->setMaxThreadCount(QThread::idealThreadCount());

	mType = ViAudio::Undefined;
	mStopped = false;
	mMultiShot = false;
    mNoiseDetector = NULL;

    mChannelMode = mode;
    mProcessMode = ViProcessor::All;

    mTotalChannels = 0;
    mCurrentChannel = 0;

	mTotalSize = 0;
	mProcessedSize = 0;

	mThread.setProcessor(this);
}

ViProcessor::~ViProcessor()
{
    if(mNoiseDetector != NULL)
    {
        delete mNoiseDetector;
        mNoiseDetector = NULL;
    }

	delete mThreadPool;
}

void ViProcessor::run()
{
	QMutexLocker locker(&mMutex);
	int channel = mCurrentChannel;
	++mCurrentChannel;
	if(mProcessMode == ViProcessor::All || isNoisy())
	{
		locker.unlock();
		execute(channel);
	}
}

void ViProcessor::startThread()
{
	mTotalSize = ViAudioPosition::convertPosition(mData.bufferSize(), ViAudioPosition::Bytes, ViAudioPosition::Samples, format());
	if(!mThread.isRunning())
	{
		mThread.start();
	}
}

void ViProcessor::executeThread()
{
	if(mThreadMutex.tryLock())
	{
		if(mStopped)
		{
			exit();
		}
		else
		{
			int size, count = usedChannelCount();
			while(!mStopped && (size = readNext()))
			{
				mProcessedSize += size;
				for(int i = 0; i < count; ++i)
				{
					//run();
					mThreadPool->start(this);
				}
				mThreadPool->waitForDone();
				setProgress((mProcessedSize * 99.0) / mTotalSize);
				mThreadMutex.unlock();
			}
			if(mStopped || !mMultiShot)
			{
				exit();
			}
		}
		mThreadMutex.unlock();
	}
}

int ViProcessor::readNext()
{
	mCurrentChannel = 0;
    if(mData.hasData())
	{
		return mData.read().size();
    }
	return 0;
}

void ViProcessor::handleExit()
{
}

bool ViProcessor::initializeProcess(ViAudioObjectPointer audioObject, ViAudio::Type type)
{
	emit started();
    setProgress(0);

	mObject = audioObject;
	mType = type;
	mStopped = false;

    mCurrentChannel = 0;
    mTotalChannels = format().channelCount();

	mTotalSize = 0;
	mProcessedSize = 0;

	if(mType != ViAudio::Undefined && mObject->hasBuffer(mType))
	{
        mData.setBuffer(mObject->buffer(mType));
		QObject::connect(mData.buffer(), SIGNAL(changed()), this, SLOT(startThread()), Qt::UniqueConnection);
		return true;
	}
	else
	{
		LOG("Unable to create the read stream.", QtCriticalMsg);
		setProgress(100);
		emit finished();
		return false;
	}
}

void ViProcessor::process(ViAudioObjectPointer audioObject, ViAudio::Type type)
{
	if(initializeProcess(audioObject, type))
	{
		initialize();
		startThread();
	}
}

void ViProcessor::setNoiseDetector(ViNoiseDetector *detector)
{
    if(mNoiseDetector == NULL)
    {
        delete mNoiseDetector;
    }
    mNoiseDetector = detector;
	mNoiseDetector->setMode(mChannelMode);
}

bool ViProcessor::isNoisy()
{
    if(mNoiseDetector == NULL)
    {
        LOG("No noise detector specified.", QtCriticalMsg);
        return false;
    }
    else
    {
		return mNoiseDetector->isNoisy(mData, mCurrentChannel);
    }
}

void ViProcessor::setChannelMode(ViProcessor::ChannelMode mode)
{
	QMutexLocker locker(&mMutex);
    mChannelMode = mode;
}

ViProcessor::ChannelMode ViProcessor::channelMode()
{
	QMutexLocker locker(&mMutex);
	return mChannelMode;
}

void ViProcessor::setProcessMode(ViProcessor::ProcessMode mode)
{
	QMutexLocker locker(&mMutex);
    mProcessMode = mode;
}

ViProcessor::ProcessMode ViProcessor::processMode()
{
	QMutexLocker locker(&mMutex);
	return mProcessMode;
}

int ViProcessor::channelCount()
{
	QMutexLocker locker(&mMutex);
	return mTotalChannels;
}

int ViProcessor::usedChannelCount()
{
	QMutexLocker locker(&mMutex);
	if(mChannelMode == ViProcessor::Separated)
	{
		return mTotalChannels;
	}
	return 1;
}

int ViProcessor::currentChannel()
{
	QMutexLocker locker(&mMutex);
	return mCurrentChannel;
}

void ViProcessor::initialize()
{
}

void ViProcessor::finalize()
{
}

ViAudioObjectPointer ViProcessor::object()
{
	QMutexLocker locker(&mMutex);
	return mObject;
}

ViAudio::Type ViProcessor::type()
{
	QMutexLocker locker(&mMutex);
	return mType;
}

ViAudioFormat ViProcessor::format()
{
	QMutexLocker locker(&mMutex);
	return mObject->format(mType);
}

ViAudioReadData& ViProcessor::data()
{
	QMutexLocker locker(&mMutex);
    return mData;
}

ViSampleChunk& ViProcessor::currentSamples()
{
	QMutexLocker locker(&mMutex);
	return mData.samples();
}

ViSampleChunk& ViProcessor::currentSamples(int channel)
{
	QMutexLocker locker(&mMutex);
	return mData.splitSamples(channel);
}

ViFrequencyChunk& ViProcessor::currentFrequencies()
{
	QMutexLocker locker(&mMutex);
	return mData.frequencies();
}

ViFrequencyChunk& ViProcessor::currentFrequencies(int channel)
{
	QMutexLocker locker(&mMutex);
	return mData.splitFrequencies(channel);
}

void ViProcessor::setMultiShot(bool multishot)
{
	QMutexLocker locker(&mMutex);
	mMultiShot = multishot;
}

bool ViProcessor::isMultiShot()
{
	QMutexLocker locker(&mMutex);
    return mMultiShot;
}

void ViProcessor::stop()
{
	mStopped = true;
	exit();
}

void ViProcessor::exit()
{
	handleExit();
	QObject::disconnect(mData.buffer(), SIGNAL(changed()), this, SLOT(startThread()));
	mStopped = false;
	finalize();
	setProgress(100);
	emit finished();
}

ViDualProcessor::ViDualProcessor()
	: ViProcessor()
{
	mType2 = ViAudio::Undefined;
}

ViDualProcessor::~ViDualProcessor()
{
}

int ViDualProcessor::readNext()
{
	QMutexLocker locker(&mMutex);
    mCurrentChannel = 0;
    if(mData.hasData())
    {
		return qMin(mData.read().size(), mData2.read().size());
    }
	return 0;
}

void ViDualProcessor::handleExit()
{
	QObject::disconnect(mData2.buffer(), SIGNAL(changed()), this, SLOT(startThread()));
}

void ViDualProcessor::process(ViAudioObjectPointer audioObject, ViAudio::Type type1, ViAudio::Type type2)
{
	if(initializeProcess(audioObject, type1))
    {
		mType2 = type2;

        if(mType2 != ViAudio::Undefined && mObject->hasBuffer(mType2))
		{
            mData2.setBuffer(mObject->buffer(mType2));
			QObject::connect(mData2.buffer(), SIGNAL(changed()), this, SLOT(startThread()), Qt::UniqueConnection);
			initialize();
			startThread();
		}
		else
		{
			LOG("Unable to create the second read stream.", QtCriticalMsg);
			setProgress(100);
			emit finished();
		}
	}
}

ViAudio::Type ViDualProcessor::type2()
{
	QMutexLocker locker(&mMutex);
	return mType2;
}

ViAudioFormat ViDualProcessor::format2()
{
	QMutexLocker locker(&mMutex);
    return mObject->format(mType2);
}

ViModifyProcessor::ViModifyProcessor(bool autoWrite)
	: ViProcessor()
{
	mAutoWrite = autoWrite;
	mType2 = ViAudio::Undefined;
    mModifyMode = ViModifyProcessor::All;
}

ViModifyProcessor::~ViModifyProcessor()
{
}

int ViModifyProcessor::readNext()
{
	QMutexLocker locker(&mMutex);
    mCurrentChannel = 0;
    if(mData.hasData())
    {
		int size = mData.read().size();
        if(mModifyMode == ViModifyProcessor::Noise)
        {
            mOriginalSamples.enqueue(QPair<bool, ViSampleChunk>(isNoisy(), mData.samples()));
        }
		return size;
    }
	return 0;
}

void ViModifyProcessor::process(ViAudioObjectPointer audioObject, ViAudio::Type type1, ViAudio::Type type2)
{
	if(initializeProcess(audioObject, type1))
	{
		mType2 = type2;
		if(mType2 != ViAudio::Undefined)
		{
            mData2.setBuffer(mObject->buffer(mType2));
            mOriginalSamples.clear();
			initialize();
			startThread();
		}
		else
		{
			LOG("Unable to create the write stream.", QtCriticalMsg);
			setProgress(100);
			emit finished();
		}
	}
}

void ViModifyProcessor::setModifyMode(ViModifyProcessor::ModifyMode mode)
{
	QMutexLocker locker(&mMutex);
    mModifyMode = mode;
}

ViAudio::Type ViModifyProcessor::type2()
{
	QMutexLocker locker(&mMutex);
	return mType2;
}

ViAudioFormat ViModifyProcessor::format2()
{
	QMutexLocker locker(&mMutex);
    return mObject->format(mType2);
}

ViAudioWriteData& ViModifyProcessor::data2()
{
	QMutexLocker locker(&mMutex);
	return mData2;
}

void ViModifyProcessor::write(ViSampleChunk &chunk)
{
	QMutexLocker locker(&mMutex);
	mData2.write(chunk);
}

void ViModifyProcessor::write(ViSampleChunk &chunk, int channel)
{
	QMutexLocker locker(&mMutex);
	mData2.enqueueSplitSamples(chunk, channel);
}

void ViModifyProcessor::writeScaled(ViSampleChunk &chunk)
{
	QMutexLocker locker(&mMutex);
	mData2.writeScaled(chunk);
}

void ViModifyProcessor::writeScaled(ViSampleChunk &chunk, int channel)
{
	QMutexLocker locker(&mMutex);
	mData2.enqueueSplitScaledSamples(chunk, channel);
}
