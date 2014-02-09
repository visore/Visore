#include <viaudiodata.h>
#include <visamplechanneler.h>
#include <viscaler.h>
#include <viaudioposition.h>
#include <vireversebufferstream.h>
#include <QQueue>

#define DEFAULT_SAMPLE_COUNT 2048
#define DEFAULT_SCALE_FROM -1
#define DEFAULT_SCALE_TO 1

ViAudioData::ViAudioData()
{
}

ViAudioData::~ViAudioData()
{
	clear();
}

void ViAudioData::setDefaults()
{
    mBuffer = NULL;
    mSampleCount = DEFAULT_SAMPLE_COUNT;
    mWindowSize = 0;
    mChannelCount = 0;
    mTransformer.setWindowFunction("Hann");
	mScaleFrom = -1;
	mScaleTo = 1;
	defaultOther();
}

void ViAudioData::clear()
{
	/*QMutexLocker locker(&mMutex);
	mBuffer = NULL;*/
    clearOther();
}

void ViAudioData::setBuffer(ViBuffer *buffer)
{
	QMutexLocker locker(&mMutex);
	mBuffer = buffer;
    update();
}

ViBuffer* ViAudioData::buffer()
{
	QMutexLocker locker(&mMutex);
    return mBuffer;
}

qint64 ViAudioData::bufferSize()
{
	QMutexLocker locker(&mMutex);
	if(mBuffer == NULL)
	{
		return 0;
	}
	return mBuffer->size();
}

qint64 ViAudioData::bufferSamples()
{
	QMutexLocker locker(&mMutex);
	if(mBuffer == NULL)
	{
		return 0;
	}
	return ViAudioPosition::convertPosition(mBuffer->size(), ViAudioPosition::Bytes, ViAudioPosition::Samples, mBuffer->format());
}

void ViAudioData::setSampleCount(int samples)
{
	QMutexLocker locker(&mMutex);
    mSampleCount = samples;
    update();
}

bool ViAudioData::setWindowFunction(QString function)
{
	return mTransformer.setWindowFunction(function);
}

int ViAudioData::sampleCount()
{
	QMutexLocker locker(&mMutex);
    return mSampleCount;
}

int ViAudioData::channelCount()
{
	QMutexLocker locker(&mMutex);
    return mChannelCount;
}

qint64 ViAudioData::position()
{
	return mStream->position();
}

void ViAudioData::setScaleRange(int from, int to)
{
	mScaleFrom = from;
	mScaleTo = to;
}

int ViAudioData::windowSize()
{
	QMutexLocker locker(&mMutex);
    return mWindowSize;
}

void ViAudioData::update()
{
    if(mBuffer == NULL)
	{
        return;
    }

    ViAudioFormat format = mBuffer->format();
    mChannelCount = format.channelCount();

    int sampleSize = format.sampleSize();
    mWindowSize = mSampleCount * (sampleSize / 8);

    mConverter.setSize(sampleSize);
    mTransformer.setSize(mSampleCount);

	mSampleChunk = ViSampleChunk(mSampleCount);
	mRawChunk = ViRawChunk(mWindowSize);
	mFrequencyChunk = ViFrequencyChunk(mSampleCount);
	mChannleChunk = ViSampleChunk(mSampleCount / mChannelCount);

    updateOther();
}

void ViAudioData::clearOther()
{
}

void ViAudioData::defaultOther()
{
}

void ViAudioData::updateOther()
{
}

/*****************************************
    READ
*****************************************/

ViAudioReadData::ViAudioReadData()
	: ViAudioData()
{
	setDefaults();
}

ViAudioReadData::ViAudioReadData(ViBuffer *buffer)
	: ViAudioData()
{
	setDefaults();
	setBuffer(buffer);
}

void ViAudioReadData::setReversed(bool reverse)
{
	mStream.setNull();
	mStream = mBuffer->createReadStream(reverse);
}

bool ViAudioReadData::hasData()
{
	QMutexLocker locker(&mMutex);
    return mStream->hasData();
}

ViSampleChunk& ViAudioReadData::read()
{
	clear();
	QMutexLocker locker(&mMutex);
	mSampleChunk.setSize(mConverter.pcmToReal(mRawChunk.data(), mSampleChunk.data(), mStream->read(mRawChunk)));
    mNeedsSampleSplit = true;
    mNeedsFrequency = true;
	mNeedsSampleScale = true;
	mNeedsSampleScaleSplit = true;
	return mSampleChunk;
}

ViSampleChunk& ViAudioReadData::samples()
{
	QMutexLocker locker(&mMutex);
    return mSampleChunk;
}

ViSampleChunks& ViAudioReadData::splitSamples()
{
	QMutexLocker locker(&mMutex);
    if(mNeedsSampleSplit)
    {
        ViSampleChanneler<qreal>::split(mSampleChunk.data(), mSampleChunk.size(), mChannelCount, mSplitSampleChunks);
        mNeedsSampleSplit = false;
    }
    return mSplitSampleChunks;
}

ViSampleChunk& ViAudioReadData::splitSamples(int index)
{
    return splitSamples()[index];
}

ViSampleChunk& ViAudioReadData::scaledSamples()
{
	return scaledSamples(mScaleFrom, mScaleTo);
}

ViSampleChunks& ViAudioReadData::scaledSplitSamples()
{
	return scaledSplitSamples(mScaleFrom, mScaleTo);
}

ViSampleChunk& ViAudioReadData::scaledSplitSamples(int index)
{
	return scaledSplitSamples(index, mScaleFrom, mScaleTo);
}

ViSampleChunk& ViAudioReadData::scaledSamples(const int &scaleFrom, const int &scaleTo)
{
	QMutexLocker locker(&mMutex);
	if(mNeedsSampleScale)
	{
		ViScaler<qreal>::scale(mSampleChunk, mScaledSampleChunk, DEFAULT_SCALE_FROM, DEFAULT_SCALE_TO, scaleFrom, scaleTo);
		mNeedsSampleScale = false;
		mNeedsSampleScaleSplit = true;
	}
	return mScaledSampleChunk;
}

ViSampleChunks& ViAudioReadData::scaledSplitSamples(const int &scaleFrom, const int &scaleTo)
{
	QMutexLocker locker(&mMutex);
	if(mNeedsSampleScaleSplit)
	{
		locker.unlock();
		scaledSamples(scaleFrom, scaleTo);
		locker.relock();
		ViSampleChanneler<qreal>::split(mScaledSampleChunk.data(), mScaledSampleChunk.size(), mChannelCount, mScaledSplitSampleChunks);
		mNeedsSampleScaleSplit = false;
	}
	return mScaledSplitSampleChunks;
}

ViSampleChunk& ViAudioReadData::scaledSplitSamples(int index, const int &scaleFrom, const int &scaleTo)
{
	return scaledSplitSamples(scaleFrom, scaleTo)[index];
}

ViFrequencyChunk& ViAudioReadData::frequencies()
{
	QMutexLocker locker(&mMutex);
    if(mNeedsFrequency)
    {
        ViChunk<qreal>::copyData(mSampleChunk, mTemporaryChunk);
        mTransformer.pad(mTemporaryChunk.data(), mSampleChunk.size());
        mTransformer.forwardTransform(mTemporaryChunk.data(), mFrequencyChunk.data());
        mNeedsFrequency = false;
    }
    return mFrequencyChunk;
}

ViFrequencyChunks& ViAudioReadData::splitFrequencies()
{
	QMutexLocker locker(&mMutex);
    if(mNeedsFrequencySplit)
    {
		ViSampleChunks& splits = splitSamples();
		ViFrequencyChunk temp(mSampleCount / mChannelCount);
		for(int i = 0; i < splits.size(); ++i)
		{
			ViSampleChunk &chunk = mSplitSampleChunks[i];
			ViChunk<qreal>::copyData(chunk, mChannleChunk);
			mTransformer.pad(mChannleChunk.data(), chunk.size());
			mTransformer.forwardTransform(mChannleChunk.data(), temp.data());
			mSplitFrequencyChunks.append(temp);
			mNeedsFrequencySplit = true;
		}
    }
    return mSplitFrequencyChunks;
}

ViFrequencyChunk& ViAudioReadData::splitFrequencies(int index)
{
    return splitFrequencies()[index];
}

void ViAudioReadData::clearOther()
{
    mSplitSampleChunks.clear();
    mNeedsSampleSplit = true;
    mNeedsFrequencySplit = true;
    mNeedsFrequency = true;
	mNeedsSampleScale = true;
	mNeedsSampleScaleSplit = true;
}

void ViAudioReadData::defaultOther()
{
	mNeedsSampleSplit = true;
	mNeedsFrequencySplit = true;
	mNeedsFrequency = true;
	mNeedsSampleScale = true;
	mNeedsSampleScaleSplit = true;
}

void ViAudioReadData::updateOther()
{
    mStream = mBuffer->createReadStream();
	mTemporaryChunk = ViFrequencyChunk(mSampleCount);
}

/*****************************************
    WRITE
*****************************************/

ViAudioWriteData::ViAudioWriteData()
	: ViAudioData()
{
	setDefaults();
}

ViAudioWriteData::ViAudioWriteData(ViBuffer *buffer)
	: ViAudioData()
{
	setDefaults();
	setBuffer(buffer);
}

void ViAudioWriteData::enqueueSplitSamples(ViSampleChunk &samples, const int &channel)
{
	QMutexLocker locker(&mMutex);
	mChannelSamples[channel].enqueue(samples);
	locker.unlock();
	dequeueSamples();
}

void ViAudioWriteData::enqueueSplitScaledSamples(ViSampleChunk &samples, const int &channel)
{
	QMutexLocker locker(&mMutex);
	ViScaler<qreal>::scale(samples, mChannleChunk, mScaleFrom, mScaleTo, DEFAULT_SCALE_FROM, DEFAULT_SCALE_TO);
	mChannelSamples[channel].enqueue(mChannleChunk);
	locker.unlock();
	dequeueSamples();
}

void ViAudioWriteData::enqueueSplitFrequencies(ViFrequencyChunk &frequencies, const int &channel)
{
	QMutexLocker locker(&mMutex);
	mTransformer.inverseTransform(frequencies.data(), mChannleChunk.data());
	mTransformer.rescale(mChannleChunk.data());
	locker.unlock();
	enqueueSplitSamples(mChannleChunk, channel);
}

void ViAudioWriteData::dequeueSamples()
{
	QMutexLocker locker(&mMutex);
	for(int i = 0; i < mChannelCount; ++i)
	{
		if(mChannelSamples[i].isEmpty())
		{
			return;
		}
	}
	ViSampleChunks temp;
	for(int i = 0; i < mChannelCount; ++i)
	{
		temp.append(mChannelSamples[i].dequeue());
	}
	locker.unlock();
	writeSplitSamples(temp);
}

void ViAudioWriteData::write(ViSampleChunk &chunk)
{
	QMutexLocker locker(&mMutex);
	mStream->write(mRawChunk.data(), mConverter.realToPcm(chunk.data(), mRawChunk.data(), chunk.size()));
}

void ViAudioWriteData::writeScaled(ViSampleChunk &chunk)
{
	QMutexLocker locker(&mMutex);
	ViScaler<qreal>::scale(chunk, mSampleChunk, mScaleFrom, mScaleTo, DEFAULT_SCALE_FROM, DEFAULT_SCALE_TO);
	locker.unlock();
	write(mSampleChunk);
}

void ViAudioWriteData::writeFrequencies(ViFrequencyChunk &frequencies)
{
	QMutexLocker locker(&mMutex);
    mTransformer.inverseTransform(frequencies.data(), mSampleChunk.data());
    mTransformer.rescale(mSampleChunk.data());
	locker.unlock();
    write(mSampleChunk);
}

void ViAudioWriteData::writeSplitSamples(ViSampleChunks &samples)
{
	QMutexLocker locker(&mMutex);
    ViSampleChanneler<qreal>::merge(samples, mSampleChunk);
	locker.unlock();
    write(mSampleChunk);
}

void ViAudioWriteData::writeSplitScaledSamples(ViSampleChunks &samples)
{
	QMutexLocker locker(&mMutex);
	ViSampleChanneler<qreal>::merge(samples, mSampleChunk);
	locker.unlock();
	writeScaled(mSampleChunk);
}

void ViAudioWriteData::writeSplitFrequencies(ViFrequencyChunks &frequencies)
{
	QMutexLocker locker(&mMutex);
    ViSampleChanneler<qreal>::merge(frequencies, mFrequencyChunk);
	locker.unlock();
    writeFrequencies(mFrequencyChunk);
}

void ViAudioWriteData::clearOther()
{
	mChannelSamples.clear();
}

void ViAudioWriteData::updateOther()
{
    mStream = mBuffer->createWriteStream();
	mChannelSamples.clear();
	for(int i = 0; i < mChannelCount; ++i)
	{
		mChannelSamples.append(QQueue<ViSampleChunk>());
	}
	//mChannelSamples.resize(mChannelCount);
}
