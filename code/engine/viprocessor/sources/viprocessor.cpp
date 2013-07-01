#include <viprocessor.h>
#include <vipowercalculator.h>
#include <vinoisedetector.h>

ViProcessor::ViProcessor(ViProcessor::ChannelMode mode)
    : QRunnable()
{
    setAutoDelete(false);
	mThreadPool = new QThreadPool();
	mThreadPool->setMaxThreadCount(QThread::idealThreadCount());

	mType = ViAudio::Undefined;
	mExit = false;
	mMultiShot = false;
    mNoiseDetector = NULL;

    mChannelMode = mode;
    mProcessMode = ViProcessor::All;

    mTotalChannels = 0;
    mCurrentChannel = 0;
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
    executeNext();
}

void ViProcessor::startThread()
{
	if(mThreadMutex.tryLock())
	{
		mThreadPool->waitForDone();
		if(mExit || !mMultiShot)
		{
			handleExit();
			QObject::disconnect(mData.buffer(), SIGNAL(changed()), this, SLOT(startThread()));
			mExit = false;
			finalize();
			setProgress(100);
			emit finished();
		}
		else
		{
			int count = 1;
			if(mChannelMode == ViProcessor::Separated)
			{
				count = mTotalChannels;
			}
			if(readNext())
			{
				for(int i = 0; i < count; ++i)
				{
					mThreadPool->start(this);
				}
				mThreadMutex.unlock();
				startThread();
			}
		}
		mThreadMutex.unlock();
	}
}

void ViProcessor::executeNext()
{
    QMutexLocker locker(&mMutex);
    if(mCurrentChannel < mTotalChannels)
    {
        if(mProcessMode == ViProcessor::All || isNoisy())
        {
            locker.unlock();
            execute(mCurrentChannel);
            locker.relock();
        }
        ++mCurrentChannel;
    }
}

bool ViProcessor::readNext()
{
	mCurrentChannel = 0;
    if(mData.hasData())
	{
        qint64 totalSize = ViAudioPosition::convertPosition(mData.bufferSize(), ViAudioPosition::Bytes, ViAudioPosition::Samples, format());
		qint64 processedSize = mData.read().size();
        setProgress((processedSize * 99.0) / totalSize);
		return true;
    }
    return false;
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
    mExit = false;

    mCurrentChannel = 0;
    mTotalChannels = format().channelCount();

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
    mNoiseDetector->setProcessor(this);
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
        return mNoiseDetector->isNoisy();
    }
}

void ViProcessor::setChannelMode(ViProcessor::ChannelMode mode)
{
    mChannelMode = mode;
}

void ViProcessor::setProcessMode(ViProcessor::ProcessMode mode)
{
    mProcessMode = mode;
}

void ViProcessor::initialize()
{
}

void ViProcessor::finalize()
{
}

ViAudioObjectPointer ViProcessor::object()
{
	return mObject;
}

ViAudio::Type ViProcessor::type()
{
	return mType;
}

ViAudioFormat ViProcessor::format()
{
	return mObject->format(mType);
}

ViAudioReadData& ViProcessor::data()
{
    return mData;
}

ViSampleChunk& ViProcessor::currentSamples()
{
    if(mChannelMode == ViProcessor::Combined)
    {
        return mData.samples();
    }
    return mData.splitSamples(mCurrentChannel);
}

ViFrequencyChunk& ViProcessor::currentFrequencies()
{
    if(mChannelMode == ViProcessor::Combined)
    {
        return mData.frequencies();
    }
    return mData.splitFrequencies(mCurrentChannel);
}

void ViProcessor::setMultiShot(bool multishot)
{
	mMultiShot = multishot;
}

bool ViProcessor::isMultiShot()
{
    return mMultiShot;
}

void ViProcessor::stop()
{
	exit();
}

void ViProcessor::exit(bool exit)
{
    mExit = true;
    if(mExit) startThread(); // Ensure the buffer is disconnected
}

ViDualProcessor::ViDualProcessor()
	: ViProcessor()
{
	mType2 = ViAudio::Undefined;
}

ViDualProcessor::~ViDualProcessor()
{
}

bool ViDualProcessor::readNext()
{
    QMutexLocker locker(&mMutex);
    mCurrentChannel = 0;
    if(mData.hasData())
    {
        qint64 totalSize = ViAudioPosition::convertPosition(qMin(mData.bufferSize(), mData2.bufferSize()), ViAudioPosition::Bytes, ViAudioPosition::Samples, format());
        qint64 processedSize = qMin(mData.read().size(), mData2.read().size());
        setProgress((processedSize * 99.0) / totalSize);
        return true;
    }
    return false;
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
	return mType2;
}

ViAudioFormat ViDualProcessor::format2()
{
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

bool ViModifyProcessor::readNext()
{
    QMutexLocker locker(&mMutex);
    mCurrentChannel = 0;
    if(mData.hasData())
    {
        qint64 totalSize = ViAudioPosition::convertPosition(mData.bufferSize(), ViAudioPosition::Bytes, ViAudioPosition::Samples, format());
        qint64 processedSize = mData.read().size();
        setProgress((processedSize * 99.0) / totalSize);
        if(mModifyMode == ViModifyProcessor::Noise)
        {
            mOriginalSamples.enqueue(QPair<bool, ViSampleChunk>(isNoisy(), mData.samples()));
        }
        return true;
    }
    return false;
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
    mModifyMode = mode;
}

ViAudio::Type ViModifyProcessor::type2()
{
	return mType2;
}

ViAudioFormat ViModifyProcessor::format2()
{
    return mObject->format(mType2);
}
