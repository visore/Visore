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

	mHasScale = false;
	mScaleFrom = 0;
	mScaleTo = 0;

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

void ViProcessor::clear()
{
	mObject.setNull();
	mData.clear();
	mType = ViAudio::Undefined;
	mStopped = false;
	mCurrentChannel = 0;
	mTotalChannels = 0;
	mTotalSize = 0;
	mProcessedSize = 0;
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

void ViProcessor::exit()
{
	handleExit();
	if(mData.buffer() != NULL)
	{
		QObject::disconnect(mData.buffer(), SIGNAL(changed()), this, SLOT(startThread()));
		mData.clear();
	}
	mStopped = false;
	finalize();
	setProgress(100);
	mObject.setNull();
	emit finished();
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
			if(count > 1)
			{
				while(!mStopped && (size = readNext()))
				{
					mProcessedSize += size;
					for(int i = 0; i < count; ++i) mThreadPool->start(this);
					mThreadPool->waitForDone();
					setProgress((mProcessedSize * 99.0) / mTotalSize);
					mThreadMutex.unlock();
				}
			}
			else
			{
				while(!mStopped && (size = readNext()))
				{
					mProcessedSize += size;
					run();
					setProgress((mProcessedSize * 99.0) / mTotalSize);
					mThreadMutex.unlock();
				}
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

	if(mHasScale)
	{
		mData.setScaleRange(mScaleFrom, mScaleTo);
	}

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

void ViProcessor::scaleSamples(qreal from, qreal to)
{
	mHasScale = true;
	mScaleFrom = from;
	mScaleTo = to;
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
	if(mNoiseDetector != NULL)
    {
        delete mNoiseDetector;
    }
    mNoiseDetector = detector;
	if(mNoiseDetector != NULL)
	{
		mNoiseDetector->setMode(mChannelMode);
	}
}

bool ViProcessor::isNoisy()
{
	return isNoisy(mCurrentChannel);
}

bool ViProcessor::isNoisy(int channel)
{
	if(mNoiseDetector == NULL)
	{
		LOG("No noise detector specified.", QtCriticalMsg);
		return false;
	}
	else
	{
		return mNoiseDetector->isNoisy(mData, channel);
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

void ViProcessor::setWindowSize(int samples)
{
	mData.setSampleCount(samples);
}

bool ViProcessor::setWindowFunction(QString function)
{
	return mData.setWindowFunction(function);
}

int ViProcessor::sampleCount()
{
	return mData.sampleCount();
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
	if(mHasScale)
	{
		return mData.scaledSamples();
	}
	return mData.samples();
}

ViSampleChunk& ViProcessor::currentSamples(int channel)
{
	QMutexLocker locker(&mMutex);
	if(mHasScale)
	{
		return mData.scaledSplitSamples(channel);
	}
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
	mThreadMutex.lock();
	mStopped = true;
	mThreadMutex.unlock();
	while(mThread.isRunning());
	exit();
}

ViElement ViProcessor::exportData()
{
	ViElement root("processor");

	ViElement window("windowsize");
	window.addChild("samples", mData.sampleCount());
	window.addChild("bytes", mData.windowSize());
	root.addChild(window);

	ViElement mode("mode");
	mode.addChild("channels", (mChannelMode == ViProcessor::Combined) ? "combined" : "separated");
	mode.addChild("process", (mProcessMode == ViProcessor::All) ? "all" : "noise");
	root.addChild(mode);

	return root;
}

bool ViProcessor::importData(ViElement element)
{
	/*if(element.name() != "processor" && element.hasChild("processor"))
	{
		element = element.child("processor");
	}
	if(element.name() != "processor")
	{
		return false;
	}

	bool success = true;

	if(element.hasChild("windowsize"))
	{
		ViElement &window = element.child("windowsize");
		if(window.hasChild("bytes"))
		{
			//mData.
		}
		else
		{
			return false;
		}
	}
	else
	{
		success = false;
	}
*/
	return false;
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

void ViDualProcessor::setWindowSize(int samples)
{
	ViProcessor::setWindowSize(samples);
	mData2.setSampleCount(samples);
}

bool ViDualProcessor::setWindowFunction(QString function)
{
	ViProcessor::setWindowFunction(function);
	mData2.setWindowFunction(function);
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

ViAudioReadData& ViDualProcessor::data2()
{
	QMutexLocker locker(&mMutex);
	return mData2;
}

ViSampleChunk& ViDualProcessor::currentSamples2()
{
	QMutexLocker locker(&mMutex);
	if(mHasScale)
	{
		return mData2.scaledSamples();
	}
	return mData2.samples();
}

ViSampleChunk& ViDualProcessor::currentSamples2(int channel)
{
	QMutexLocker locker(&mMutex);
	if(mHasScale)
	{
		return mData2.scaledSplitSamples(channel);
	}
	return mData2.splitSamples(channel);
}

ViFrequencyChunk& ViDualProcessor::currentFrequencies2()
{
	QMutexLocker locker(&mMutex);
	return mData2.frequencies();
}

ViFrequencyChunk& ViDualProcessor::currentFrequencies2(int channel)
{
	QMutexLocker locker(&mMutex);
	return mData2.splitFrequencies(channel);
}

ViModifyProcessor::ViModifyProcessor()
	: ViProcessor()
{
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
			bool noisy;
			if(mChannelMode == ViProcessor::Separated)
			{
				for(int i = 0; i < mTotalChannels; ++i)
				{
					noisy = isNoisy(i);
					mNoiseData.enqueueSplitScaledSamples(mNoiseDetector->noise().data(), i);
					mNoiseMaskData.enqueueSplitScaledSamples(mNoiseDetector->noise().mask(), i);
					mOriginalData.enqueue(noisy, mData.splitSamples(i), i);
				}
			}
			else
			{
				noisy = isNoisy();
				mNoiseData.writeScaled(mNoiseDetector->noise().data());
				mNoiseMaskData.writeScaled(mNoiseDetector->noise().mask());
				mOriginalData.enqueue(noisy, mData.samples());
			}
        }
		return size;
    }
	return 0;
}

void ViModifyProcessor::process(ViAudioObjectPointer audioObject, ViAudio::Type type1, ViAudio::Type type2)
{
	if(initializeProcess(audioObject, type1))
	{
		if(mHasScale)
		{
			mData2.setScaleRange(mScaleFrom, mScaleTo);
		}
		mType2 = type2;
		if(mType2 != ViAudio::Undefined)
		{
            mData2.setBuffer(mObject->buffer(mType2));
            mOriginalData.clear();
			mOriginalData.setChannels(mTotalChannels);

			mNoiseData.setBuffer(mObject->buffer(ViAudio::Noise));
			mNoiseData.setScaleRange(0, 1);

			mNoiseMaskData.setBuffer(mObject->buffer(ViAudio::NoiseMask));
			mNoiseMaskData.setScaleRange(0, 1);

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

void ViModifyProcessor::setWindowSize(int samples)
{
	ViProcessor::setWindowSize(samples);
	mData2.setSampleCount(samples);
}

bool ViModifyProcessor::setWindowFunction(QString function)
{
	LOG("Inverse window function still has to be implemented in the fourier transformer.", QtFatalMsg);
	ViProcessor::setWindowFunction(function);
	mData2.setWindowFunction(function);
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
	if(mModifyMode == ViModifyProcessor::Noise && !mOriginalData.dequeueNoisy())
	{
		mData2.write(mOriginalData.dequeue());
	}
	else if(mHasScale)
	{
		mData2.writeScaled(chunk);
	}
	else
	{
		mData2.write(chunk);
	}
}

void ViModifyProcessor::write(ViSampleChunk &chunk, int channel)
{
	QMutexLocker locker(&mMutex);
	if(mModifyMode == ViModifyProcessor::Noise && !mOriginalData.dequeueNoisy(channel))
	{
		mData2.enqueueSplitSamples(mOriginalData.dequeue(channel), channel);
	}
	else if(mHasScale)
	{
		mData2.enqueueSplitScaledSamples(chunk, channel);
	}
	else
	{
		mData2.enqueueSplitSamples(chunk, channel);
	}
}

void ViModifyProcessor::writeFrequencies(ViFrequencyChunk &chunk)
{
	QMutexLocker locker(&mMutex);
	if(mModifyMode == ViModifyProcessor::Noise && !mOriginalData.dequeueNoisy())
	{
		mData2.write(mOriginalData.dequeue());
	}
	else
	{
		mData2.writeFrequencies(chunk);
	}
}

void ViModifyProcessor::writeFrequencies(ViFrequencyChunk &chunk, int channel)
{
	QMutexLocker locker(&mMutex);
	if(mModifyMode == ViModifyProcessor::Noise && !mOriginalData.dequeueNoisy(channel))
	{
		mData2.enqueueSplitSamples(mOriginalData.dequeue(channel), channel);
	}
	else
	{
		mData2.enqueueSplitFrequencies(chunk, channel);
	}
}

ViElement ViModifyProcessor::exportData()
{
	ViElement root = ViProcessor::exportData();
	root.child("mode").addChild("modify", (mModifyMode == ViModifyProcessor::All) ? "all" : "noise");
	return root;
}

bool ViModifyProcessor::importData(ViElement element)
{
	if(ViProcessor::importData(element))
	{
		if(element.hasChild("mode"))
		{
			if(element.child("mode").hasChild("modify"))
			{
				QString value = element.child("mode").child("modify").toString();
				if(value == "all")
				{
					setModifyMode(ViModifyProcessor::All);
					return true;
				}
				else if(value == "noise")
				{
					setModifyMode(ViModifyProcessor::Noise);
					return true;
				}
			}
		}
	}
	return false;
}


ViModifyData::ViModifyData()
{
	setChannels(1);
}

void ViModifyData::setChannels(const int &channels)
{
	mChannels = channels;
}

void ViModifyData::clear()
{
	mNoise.clear();
	mUsed.clear();
	mData.clear();
}

void ViModifyData::enqueue(const bool &noisy, const ViSampleChunk &data, const int &channel)
{
	if(mUsed.isEmpty() || mUsed.last()[channel] != ViModifyData::NotAdded)
	{
		QVector<ViModifyData::Usage> usage(mChannels);
		for(int i = 0; i < mChannels; ++i)
		{
			usage[i] = ViModifyData::NotAdded;
		}
		mNoise.enqueue(QVector<bool>(mChannels));
		mUsed.enqueue(usage);
		mData.enqueue(QVector<ViSampleChunk>(mChannels));
	}

	int lastUnusedIndex = 0;
	for(int i = mUsed.size() - 1; i >= 0; --i)
	{
		if(mUsed[i][channel] != ViModifyData::NotAdded)
		{
			lastUnusedIndex = i + 1;
			break;
		}
	}

	mNoise[lastUnusedIndex][channel] = noisy;
	mUsed[lastUnusedIndex][channel] = ViModifyData::Added;
	mData[lastUnusedIndex][channel] = data;

	//Ensure old written values are removed.
	int written;
	do
	{
		written = 0;
		QVector<ViModifyData::Usage> &usage = mUsed.first();
		for(int i = 0; i < usage.size(); ++i)
		{
			if(usage[i] == ViModifyData::Written)
			{
				++written;
			}
			else
			{
				break;
			}
		}
		if(written == mChannels)
		{
			mNoise.dequeue();
			mUsed.dequeue();
			mData.dequeue();
		}
	}
	while(written == mChannels);
}

bool ViModifyData::dequeueNoisy(const int &channel)
{
	for(int i = 0; i < mUsed.size(); ++i)
	{
		if(mUsed[i][channel] == ViModifyData::Added)
		{
			if(mNoise[i][channel])
			{
				dequeue(channel);
				return true;
			}
			return false;
		}
	}
	return true;
}

ViSampleChunk& ViModifyData::dequeue(const int &channel)
{
	for(int i = 0; i < mUsed.size(); ++i)
	{
		if(mUsed[i][channel] == ViModifyData::Added)
		{
			mUsed[i][channel] = ViModifyData::Written;
			return mData[i][channel];
		}
	}
	LOG("Trying to use a non-existing chunk.", QtFatalMsg);
}
