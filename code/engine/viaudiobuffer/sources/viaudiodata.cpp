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
    mRawChunk.size();//For some reasons this prevents seg faults in delete mRawChunck
    clear();
}

void ViAudioData::setDefaults()
{
    mBuffer = NULL;
    mWindowSize = 0;
    mChannelCount = 0;
	mSampleSize = 0;
    mTransformer.setWindowFunction("Hann");
	mScaleFrom = -1;
	mScaleTo = 1;
	setSampleCount(DEFAULT_SAMPLE_COUNT);
	defaultOther();
}

void ViAudioData::clear()
{
	QMutexLocker locker(&mMutex);
	mStream.isNull();
	mBuffer = NULL;
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
    mTransformer.setSize(samples);
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

	mSampleSize = format.sampleSize();
	mWindowSize = mSampleCount * (mSampleSize / 8);

	mConverter.setSize(mSampleSize);
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

ViAudioReadData::ViAudioReadData(ViBuffer *buffer, const bool &reversed)
	: ViAudioData()
{
	setDefaults();
	mReversed = reversed;
	setBuffer(buffer);
}

void ViAudioReadData::setReversed(const bool &reversed)
{
	QMutexLocker locker(&mMutex);
	mReversed = reversed;
	update();
}

bool ViAudioReadData::hasData()
{
	QMutexLocker locker(&mMutex);
    return mStream->hasData();
}

ViSampleChunk& ViAudioReadData::read()
{
	clearOther();
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
	mSplitFrequencyChunks.clear();
	mScaledSplitSampleChunks.clear();
    mNeedsSampleSplit = true;
    mNeedsFrequencySplit = true;
    mNeedsFrequency = true;
	mNeedsSampleScale = true;
	mNeedsSampleScaleSplit = true;
}

void ViAudioReadData::defaultOther()
{
	mReversed = false;
	mNeedsSampleSplit = true;
	mNeedsFrequencySplit = true;
	mNeedsFrequency = true;
	mNeedsSampleScale = true;
	mNeedsSampleScaleSplit = true;
}

void ViAudioReadData::updateOther()
{
	mStream = mBuffer->createReadStream(mReversed);
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
	mChannelSamples[channel].append(samples);
	locker.unlock();
	dequeueSamples();
}

void ViAudioWriteData::enqueueSplitScaledSamples(ViSampleChunk &samples, const int &channel)
{
	QMutexLocker locker(&mMutex);
	ViScaler<qreal>::scale(samples, mChannleChunk, mScaleFrom, mScaleTo, DEFAULT_SCALE_FROM, DEFAULT_SCALE_TO);
	mChannelSamples[channel].append(mChannleChunk);
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

qint64 ViAudioWriteData::writableSize()
{
	QMutexLocker locker(&mMutex);
	qint64 minSize = LONG_MAX;
	for(int i = 0; i < mChannelCount; ++i)
	{
		if(mChannelSamples[i].size() < minSize) minSize = mChannelSamples[i].size();
	}
	if(minSize == LONG_MAX) minSize = 0;
	return minSize;
}

void ViAudioWriteData::dequeueSamples()
{
	qint64 minSize = writableSize();
	QMutexLocker locker(&mMutex);
	if(minSize == 0) return;

	ViSampleChunks temp;
	for(int i = 0; i < mChannelCount; ++i)
	{
		if(mChannelSamples[i].size() == minSize)
		{
			temp.append(mChannelSamples[i]);
			mChannelSamples[i].clear();
		}
		else
		{
			temp.append(mChannelSamples[i].subset(0, minSize));
			mChannelSamples[i].removeFirst(minSize);
		}
	}
	locker.unlock();
	writeSplitSamples(temp);
}

void ViAudioWriteData::write(ViSampleChunk &chunk)
{
	QMutexLocker locker(&mMutex);
	mRawChunk.resize(chunk.size() * (mSampleSize / 8));
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
    //mChannelSamples.clear();
}

void ViAudioWriteData::updateOther()
{
	mStream = mBuffer->createWriteStream();
	mChannelSamples.clear();
	for(int i = 0; i < mChannelCount; ++i)
	{
		mChannelSamples.append(ViSampleChunk());
	}
	//mChannelSamples.resize(mChannelCount);
}
