#include <viprocessor.h>
#include <vipowercalculator.h>

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
	mType = ViAudioObject::Undefined;
	mChunkSize = CHUNK_SIZE;
	mThread.setProcessor(this);
	mProgressEnabled = true;
	mExit = false;
	mMultiShot = false;
}

ViProcessor::~ViProcessor()
{
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
		execute();
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
		QObject::connect(mReadStream->buffer(), SIGNAL(changed()), this, SLOT(startThread()), Qt::UniqueConnection);
	}
}

void ViProcessor::startProgressless()
{
	while(hasData1() && !mExit)
	{
		read1();
		execute();
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
		QObject::connect(mReadStream->buffer(), SIGNAL(changed()), this, SLOT(startThread()), Qt::UniqueConnection);
	}
}

bool ViProcessor::initializeProcess(ViAudioObjectPointer audioObject, ViAudioObject::Type type)
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

	mChunk.resize(mChunkSize);
	if(mType != ViAudioObject::Undefined && mObject->hasBuffer(mType))
	{
		mReadStream = mObject->buffer(mType)->createReadStream();
		int sampleSize = mObject->buffer(mType)->format().sampleSize();
		mConverter.setSize(sampleSize);
		mSamples.resize(mChunkSize / (sampleSize / 8));
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

void ViProcessor::process(ViAudioObjectPointer audioObject)
{
	process(audioObject, audioObject->inputType());
}

void ViProcessor::process(ViAudioObjectPointer audioObject, ViAudioObject::Type type)
{
	if(initializeProcess(audioObject, type))
	{
		initialize();
		mThread.start();
	}
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

ViAudioObject::Type ViProcessor::type()
{
	return type1();
}

ViAudioObject::Type ViProcessor::type1()
{
	return mType;
}

ViSampleChunk& ViProcessor::read()
{
	return read1();
}

ViSampleChunk& ViProcessor::read1()
{
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
	mExit = exit;
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
	return mSamples.size();
}

ViDualProcessor::ViDualProcessor()
	: ViProcessor()
{
	mType2 = ViAudioObject::Undefined;
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
		execute();
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
		execute();
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

void ViDualProcessor::process(ViAudioObjectPointer audioObject)
{
	process(audioObject, audioObject->inputType(), audioObject->outputType());
}

void ViDualProcessor::process(ViAudioObjectPointer audioObject, ViAudioObject::Type type1)
{
	process(audioObject, type1, audioObject->outputType());
}

void ViDualProcessor::process(ViAudioObjectPointer audioObject, ViAudioObject::Type type1, ViAudioObject::Type type2)
{
	if(initializeProcess(audioObject, type1))
	{
		mType2 = type2;
		mChunk2.resize(chunkSize());

		if(mType2 != ViAudioObject::Undefined && object()->hasBuffer(mType2))
		{
			mReadStream2 = object()->buffer(mType2)->createReadStream();
			int sampleSize = object()->buffer(mType2)->format().sampleSize();
			mConverter2.setSize(sampleSize);
			mSamples2.resize(chunkSize() / (sampleSize / 8));
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

ViAudioObject::Type ViDualProcessor::type2()
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

ViModifyProcessor::ViModifyProcessor(bool autoWrite)
	: ViProcessor()
{
	mAutoWrite = autoWrite;
	mType2 = ViAudioObject::Undefined;
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
		execute();
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
		execute();
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

void ViModifyProcessor::process(ViAudioObjectPointer audioObject)
{
	process(audioObject, audioObject->inputType(), audioObject->outputType());
}

void ViModifyProcessor::process(ViAudioObjectPointer audioObject, ViAudioObject::Type type1)
{
	process(audioObject, type1, audioObject->outputType());
}

void ViModifyProcessor::process(ViAudioObjectPointer audioObject, ViAudioObject::Type type1, ViAudioObject::Type type2)
{
	if(initializeProcess(audioObject, type1))
	{
		mType2 = type2;
		mChunk2.resize(chunkSize());
		if(mType2 != ViAudioObject::Undefined)
		{
			mWriteStream = object()->buffer(mType2)->createWriteStream();
			mConverter2.setSize(object()->buffer(mType2)->format().sampleSize());
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

ViAudioObject::Type ViModifyProcessor::type2()
{
	return mType2;
}

ViAudioFormat ViModifyProcessor::format2()
{
	return object()->format(mType2);
}

void ViModifyProcessor::write()
{
	write(samples1());
}

void ViModifyProcessor::write(ViSampleChunk& samples)
{
	mWriteStream->write(mChunk2.data(), mConverter2.realToPcm(samples.data(), mChunk2.data(), samples.size()));
}
