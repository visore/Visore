#include <viprocessor.h>
#include <vipowercalculator.h>
#include <vinoisedetector.h>

#define CHUNK_SIZE 4096

ViProcessorThread::ViProcessorThread()
	: QThread()
{
	mProcessor = NULL;
}

void ViProcessorThread::setProcessor(ViProcessor *processor)
{
	//QObject::disconnect(this, SIGNAL(finished()), 0, 0);
	mProcessor = processor;
	//QObject::connect(this, SIGNAL(finished()), mProcessor, SIGNAL(finished()));
}

void ViProcessorThread::run()
{
	if(mProcessor->isProgressEnabled())
	{
		mProcessor->startProgress();
	}
	else
	{
		mProcessor->startProgressless();
	}
}

ViProcessor::ViProcessor()
{
	mType = ViAudio::Undefined;
	mChunkSize = CHUNK_SIZE;
    mSampleCount = 0;
	mThread.setProcessor(this);
	mProgressEnabled = true;
	mExit = false;
	mMultiShot = false;
    mNoiseDetector = NULL;
    mProcessMode = ViProcessor::All;

    mRecalculateFrequencies = true;
    mTransformer.setWindowFunction("Hann");
}

ViProcessor::~ViProcessor()
{
    if(mNoiseDetector != NULL)
    {
        delete mNoiseDetector;
        mNoiseDetector = NULL;
    }
}

void ViProcessor::startThread()
{
	if(!mThread.isRunning())
    {
		mThread.start();
	}
}

void ViProcessor::startProgress()
{
	qint64 totalSize = ViAudioPosition::convertPosition(mReadStream->size(), ViAudioPosition::Bytes, ViAudioPosition::Samples, format());
	qint64 processedSize = 0;
	while(hasData1() && !mExit)
	{
		processedSize += read1().size();
        if(mProcessMode == ViProcessor::All || isNoisy())
        {
            execute();
        }
		setProgress((processedSize * 99.0) / totalSize);
	}
    if(mExit || !mMultiShot)
    {
        QObject::disconnect(mReadStream->buffer(), SIGNAL(changed()), this, SLOT(startThread()));
        mExit = false;
        finalize();
        setProgress(100);
        emit finished();
    }
    else
    {
        //QObject::connect(mReadStream->buffer(), SIGNAL(changed()), this, SLOT(startThread()), Qt::UniqueConnection);
    }
}

void ViProcessor::startProgressless()
{
	while(hasData1() && !mExit)
	{
		read1();
        if(mProcessMode == ViProcessor::All || isNoisy())
        {
            execute();
        }
	}
    if(mExit || !mMultiShot)
    {
        QObject::disconnect(mReadStream->buffer(), SIGNAL(changed()), this, SLOT(startThread()));
        mExit = false;
        finalize();
        setProgress(100);
        emit finished();
    }
    else
    {

    }
}

bool ViProcessor::initializeProcess(ViAudioObjectPointer audioObject, ViAudio::Type type)
{
	emit started();
	if(mProgressEnabled)
	{
		setProgress(0);
	}
	else
	{
		setProgress(-1);
	}

	mObject = audioObject;
	mType = type;
    mExit = false;

	mChunk.resize(mChunkSize);
	if(mType != ViAudio::Undefined && mObject->hasBuffer(mType))
	{
		mReadStream = mObject->buffer(mType)->createReadStream();
        QObject::connect(mReadStream->buffer(), SIGNAL(changed()), this, SLOT(startThread()), Qt::UniqueConnection);
		int sampleSize = mObject->buffer(mType)->format().sampleSize();
		mConverter.setSize(sampleSize);

        mSampleCount = mChunkSize / (sampleSize / 8);
        mSamples.resize(mSampleCount);

        mTransformer.setSize(mSampleCount);
        mFrequencies.resize(mSampleCount);
        mTempFourierData.resize(mSampleCount);
        mRecalculateFrequencies = true;

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
		mThread.start();
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

void ViProcessor::setProcessMode(ViProcessor::ProcessMode mode)
{
    mProcessMode = mode;
}

ViProcessor::ProcessMode ViProcessor::processMode()
{
    return mProcessMode;
}

void ViProcessor::initialize()
{
}

void ViProcessor::finalize()
{
}

ViProcessorThread& ViProcessor::thread()
{
	return mThread;
}

ViAudioObjectPointer ViProcessor::object()
{
	return mObject;
}

ViBufferStreamPointer ViProcessor::readStream()
{
	return mReadStream;
}

ViAudio::Type ViProcessor::type()
{
	return type1();
}

ViAudio::Type ViProcessor::type1()
{
	return mType;
}

ViSampleChunk& ViProcessor::read()
{
	return read1();
}

ViSampleChunk& ViProcessor::read1()
{
    mRecalculateFrequencies = true;
	mSamples.setSize(mConverter.pcmToReal(mChunk.data(), mSamples.data(), mReadStream->read(mChunk)));
	return mSamples;
}

ViSampleChunk& ViProcessor::samples()
{
	return samples1();
}

ViSampleChunk& ViProcessor::samples1()
{
	return mSamples;
}

bool ViProcessor::hasData()
{
	return hasData1();
}

bool ViProcessor::hasData1()
{
	return mReadStream->hasData();
}

ViAudioFormat ViProcessor::format()
{
	return format1();
}

ViAudioFormat ViProcessor::format1()
{
	return mObject->format(mType);
}

void ViProcessor::enableProgress(bool enable)
{
	mProgressEnabled = enable;
}

void ViProcessor::disableProgress(bool disable)
{
	mProgressEnabled = !disable;
}

bool ViProcessor::isProgressEnabled()
{
	return mProgressEnabled;
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
	if(mExit)
    {
        startThread(); // Ensure the buffer is disconnected
	}
}

bool ViProcessor::willExit()
{
	return mExit;
}

void ViProcessor::setChunkSize(int size)
{
	mChunkSize = size;
}

int ViProcessor::chunkSize()
{
	return mChunkSize;
}

int ViProcessor::sampleCount()
{
    return mSampleCount;
}

int ViProcessor::sampleCount1()
{
    return mSampleCount;
}

ViFrequencyChunk& ViProcessor::frequencies()
{
    return frequencies1();
}

ViFrequencyChunk& ViProcessor::frequencies1()
{
    if(mRecalculateFrequencies)
    {
        mRecalculateFrequencies = false;
        samplesToFrequencies(mSamples, mFrequencies, mTempFourierData);
    }
    return mFrequencies;
}

void ViProcessor::samplesToFrequencies(ViSampleChunk &inputSamples, ViFrequencyChunk &outputFrequencies, ViFrequencyChunk &intermidiate)
{
    ViChunk<qreal>::copyData(inputSamples, intermidiate);
    mTransformer.pad(intermidiate.data(), inputSamples.size());

    mTransformer.forwardTransform(intermidiate.data(), outputFrequencies.data());
}

void ViProcessor::frequenciesToSamples(ViFrequencyChunk &inputFrequencies, ViSampleChunk &outputSamples)
{
    mTransformer.inverseTransform(inputFrequencies.data(), outputSamples.data());
    mTransformer.rescale(outputSamples.data());
}

ViDualProcessor::ViDualProcessor()
	: ViProcessor()
{
	mType2 = ViAudio::Undefined;
    mSampleCount2 = 0;
    mRecalculateFrequencies2 = true;
}

ViDualProcessor::~ViDualProcessor()
{
}

void ViDualProcessor::startProgress()
{
	qint64 totalSize = ViAudioPosition::convertPosition(readStream()->size(), ViAudioPosition::Bytes, ViAudioPosition::Samples, format());
	qint64 processedSize = 0;
	while(hasData1() && hasData2() && !willExit())
    {
		processedSize += qMin(read1().size(), read2().size());
        if(processMode() == ViProcessor::All || isNoisy())
        {
            execute();
        }
		setProgress((processedSize * 99.0) / totalSize);
	}
	if(willExit() || !isMultiShot())
    {
		QObject::disconnect(readStream()->buffer(), SIGNAL(changed()), this, SLOT(startThread()));
		QObject::disconnect(mReadStream2->buffer(), SIGNAL(changed()), this, SLOT(startThread()));
		exit(false);
		finalize();
		setProgress(100);
		emit finished();
	}
	else
	{
		QObject::connect(readStream()->buffer(), SIGNAL(changed()), this, SLOT(startThread()), Qt::UniqueConnection);
		QObject::connect(mReadStream2->buffer(), SIGNAL(changed()), this, SLOT(startThread()), Qt::UniqueConnection);
	}
}

void ViDualProcessor::startProgressless()
{
	while(hasData1() && hasData2() && !willExit())
    {
		read1().size();
		read2().size();
        if(processMode() == ViProcessor::All || isNoisy())
        {
            execute();
        }
	}
	if(willExit() || !isMultiShot())
	{
		QObject::disconnect(readStream()->buffer(), SIGNAL(changed()), this, SLOT(startThread()));
		QObject::disconnect(mReadStream2->buffer(), SIGNAL(changed()), this, SLOT(startThread()));
		exit(false);
		finalize();
		setProgress(100);
		emit finished();
	}
	else
	{
		QObject::connect(readStream()->buffer(), SIGNAL(changed()), this, SLOT(startThread()), Qt::UniqueConnection);
		QObject::connect(mReadStream2->buffer(), SIGNAL(changed()), this, SLOT(startThread()), Qt::UniqueConnection);
	}
}

void ViDualProcessor::process(ViAudioObjectPointer audioObject, ViAudio::Type type1, ViAudio::Type type2)
{
	if(initializeProcess(audioObject, type1))
    {
		mType2 = type2;
		mChunk2.resize(chunkSize());

		if(mType2 != ViAudio::Undefined && object()->hasBuffer(mType2))
		{
			mReadStream2 = object()->buffer(mType2)->createReadStream();
			int sampleSize = object()->buffer(mType2)->format().sampleSize();
			mConverter2.setSize(sampleSize);

            mSampleCount2 = chunkSize() / (sampleSize / 8);
            mSamples2.resize(mSampleCount2);

            mFrequencies2.resize(mSampleCount2);
            mTempFourierData2.resize(mSampleCount2);
            mRecalculateFrequencies2 = true;

			initialize();
            thread().start();
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

ViSampleChunk& ViDualProcessor::read2()
{
	mSamples2.setSize(mConverter2.pcmToReal(mChunk2.data(), mSamples2.data(), mReadStream2->read(mChunk2)));
	return mSamples2;
}

ViSampleChunk& ViDualProcessor::samples2()
{
	return mSamples2;
}

bool ViDualProcessor::hasData2()
{
	return mReadStream2->hasData();
}

ViAudioFormat ViDualProcessor::format2()
{
	return object()->format(mType2);
}

int ViDualProcessor::sampleCount2()
{
    return mSampleCount2;
}

ViFrequencyChunk& ViDualProcessor::frequencies2()
{
    if(mRecalculateFrequencies2)
    {
        mRecalculateFrequencies2 = false;
        samplesToFrequencies(mSamples2, mFrequencies2, mTempFourierData2);
    }
    return mFrequencies2;
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

void ViModifyProcessor::startProgress()
{
	qint64 totalSize = ViAudioPosition::convertPosition(readStream()->size(), ViAudioPosition::Bytes, ViAudioPosition::Samples, format());
	qint64 processedSize = 0;
	while(hasData1() && !willExit())
	{
		processedSize += read1().size();
        bool noise = false;
        if(mModifyMode == ViModifyProcessor::Noise)
        {
            noise = isNoisy();
            if(noise) LOG("Noise detected at: "+QString::number(tt)+"  "+QString::number(tt*sampleCount()));
            ++tt;
            mOriginalSamples.enqueue(QPair<bool, ViSampleChunk>(noise, samples1()));
        }
        if(processMode() == ViProcessor::All || noise)
        {
            execute();
        }
        if(mAutoWrite)
		{
			write();
		}
		setProgress((processedSize * 99.0) / totalSize);
	}
	if(willExit() || !isMultiShot())
	{
		QObject::disconnect(readStream()->buffer(), SIGNAL(changed()), this, SLOT(startThread()));
		exit(false);
		finalize();
		setProgress(100);
		emit finished();
	}
	else
	{
		QObject::connect(readStream()->buffer(), SIGNAL(changed()), this, SLOT(startThread()), Qt::UniqueConnection);
	}
}

void ViModifyProcessor::startProgressless()
{
	while(hasData1() && !willExit())
	{
		read1();
        bool noise = false;
        if(mModifyMode == ViModifyProcessor::Noise)
        {
            noise = isNoisy();
            mOriginalSamples.enqueue(QPair<bool, ViSampleChunk>(noise, samples1()));
        }
        if(processMode() == ViProcessor::All || noise)
        {
            execute();
        }
		if(mAutoWrite)
		{
			write();
		}
	}
	if(willExit() || !isMultiShot())
	{
		QObject::disconnect(readStream()->buffer(), SIGNAL(changed()), this, SLOT(startThread()));
		exit(false);
		finalize();
		setProgress(100);
		emit finished();
	}
	else
	{
		QObject::connect(readStream()->buffer(), SIGNAL(changed()), this, SLOT(startThread()), Qt::UniqueConnection);
	}
}

void ViModifyProcessor::process(ViAudioObjectPointer audioObject, ViAudio::Type type1, ViAudio::Type type2)
{tt=0;
	if(initializeProcess(audioObject, type1))
	{
		mType2 = type2;
		mChunk2.resize(chunkSize());
		if(mType2 != ViAudio::Undefined)
		{
			mWriteStream = object()->buffer(mType2)->createWriteStream();
			mConverter2.setSize(object()->buffer(mType2)->format().sampleSize());
            mOriginalSamples.clear();
			initialize();
			thread().start();
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

ViModifyProcessor::ModifyMode ViModifyProcessor::modifyMode()
{
    return mModifyMode;
}

ViAudio::Type ViModifyProcessor::type2()
{
	return mType2;
}

ViAudioFormat ViModifyProcessor::format2()
{
	return object()->format(mType2);
}

bool ViModifyProcessor::write()
{
    return write(samples1());
}

bool ViModifyProcessor::write(ViSampleChunk& samples)
{
    bool success = false;
    if(mModifyMode == ViModifyProcessor::All)
    {
        success = true;
    }
    else if(mOriginalSamples.isEmpty())
    {
        samples = samples1();
    }
    else
    {
        QPair<bool, ViSampleChunk> data = mOriginalSamples.dequeue();
        if(!data.first)
        {
            samples = data.second;
        }
    }

    mWriteStream->write(mChunk2.data(), mConverter2.realToPcm(samples.data(), mChunk2.data(), samples.size()));
    return success;
}

bool ViModifyProcessor::writeFrequencies(ViFrequencyChunk &frequencies)
{
    ViSampleChunk samples(sampleCount());
    frequenciesToSamples(frequencies, samples);
    return write(samples);
}
