#include <viaudiodata.h>
#include <visamplechanneler.h>

#define DEFAULT_SAMPLE_COUNT 2048

ViAudioData::ViAudioData()
{
    setDefaults();
}

ViAudioData::ViAudioData(ViBuffer *buffer)
{
    setDefaults();
    setBuffer(buffer);
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
}

void ViAudioData::clear()
{
	QMutexLocker locker(&mMutex);
    setDefaults();
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

int ViAudioData::bufferSize()
{
	QMutexLocker locker(&mMutex);
    if(mBuffer == NULL)
    {
        return 0;
    }
    return mBuffer->size();
}

void ViAudioData::setSampleCount(int samples)
{
	QMutexLocker locker(&mMutex);
    mSampleCount = samples;
    update();
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

    mSampleChunk.resize(mSampleCount);
    mRawChunk.resize(mWindowSize);
    mFrequencyChunk.resize(mSampleCount);

    updateOther();
}

/*****************************************
    READ
*****************************************/

bool ViAudioReadData::hasData()
{
	QMutexLocker locker(&mMutex);
    return mStream->hasData();
}

ViSampleChunk& ViAudioReadData::read()
{
	QMutexLocker locker(&mMutex);
	mSampleChunk.setSize(mConverter.pcmToReal(mRawChunk.data(), mSampleChunk.data(), mStream->read(mRawChunk)));
    mNeedsSampleSplit = true;
    mNeedsFrequency = true;
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

ViFrequencyChunk& ViAudioReadData::frequencies()
{
	QMutexLocker locker(&mMutex);
    if(mNeedsFrequency)
    {
        ViChunk<qreal>::copyData(mSampleChunk, mTemporaryChunk);
        mTransformer.pad(mTemporaryChunk.data(), mSampleChunk.size());
        mTransformer.forwardTransform(mTemporaryChunk.data(), mFrequencyChunk.data());
        mNeedsFrequency = false;
        mNeedsFrequencySplit = true;
    }
    return mFrequencyChunk;
}

ViFrequencyChunks& ViAudioReadData::splitFrequencies()
{
	QMutexLocker locker(&mMutex);
    if(mNeedsFrequencySplit)
    {
        ViSampleChanneler<qreal>::split(mFrequencyChunk.data(), mFrequencyChunk.size(), mChannelCount, mSplitFrequencyChunks);
        mNeedsFrequencySplit = false;
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
}

void ViAudioReadData::updateOther()
{
    mStream = mBuffer->createReadStream();
    mTemporaryChunk.resize(mSampleCount);
}

/*****************************************
    WRITE
*****************************************/

void ViAudioWriteData::write(ViSampleChunk &chunk)
{
	QMutexLocker locker(&mMutex);
    mStream->write(mRawChunk.data(), mConverter.realToPcm(chunk.data(), mRawChunk.data(), chunk.size()));
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

void ViAudioWriteData::writeSplitFrequencies(ViFrequencyChunks &frequencies)
{
	QMutexLocker locker(&mMutex);
    ViSampleChanneler<qreal>::merge(frequencies, mFrequencyChunk);
	locker.unlock();
    writeFrequencies(mFrequencyChunk);
}

void ViAudioWriteData::clearOther()
{
}

void ViAudioWriteData::updateOther()
{
    mStream = mBuffer->createWriteStream();
}
