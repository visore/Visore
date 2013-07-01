#include <viaudiodata.h>
#include <visamplechanneler.h>

#define DEFAULT_SAMPLE_COUNT 2048

ViAudioData::ViAudioData()
{
    clear();
}

ViAudioData::ViAudioData(ViBuffer *buffer)
{
    clear();
    setBuffer(buffer);
}

ViAudioData::~ViAudioData()
{
    clear();
}

void ViAudioData::clear()
{
    mBuffer = NULL;
    mSampleCount = DEFAULT_SAMPLE_COUNT;
    mWindowSize = 0;
    mChannelCount = 0;

    clearOther();
}

void ViAudioData::setBuffer(ViBuffer *buffer)
{
    mBuffer = buffer;
    update();
}

ViBuffer* ViAudioData::buffer()
{
    return mBuffer;
}

void ViAudioData::setSampleCount(int samples)
{
    mSampleCount = samples;
    update();
}

int ViAudioData::sampleCount()
{
    return mSampleCount;
}

int ViAudioData::channelCount()
{
    return mChannelCount;
}

int ViAudioData::windowSize()
{
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

ViSampleChunk& ViAudioReadData::read()
{
    mSampleChunk.setSize(mConverter.pcmToReal(mRawChunk.data(), mSampleChunk.data(), mStream->read(mRawChunk)));
    mNeedsSampleSplit = true;
    mNeedsFrequency = true;
}

ViSampleChunk& ViAudioReadData::samples()
{
    return mSampleChunk;
}

ViSampleChunks& ViAudioReadData::splitSamples()
{
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
    READ
*****************************************/

void ViAudioWriteData::write(ViSampleChunk &chunk)
{
    mStream->write(mRawChunk.data(), mConverter.realToPcm(chunk.data(), mRawChunk.data(), chunk.size()));
}

void ViAudioWriteData::writeFrequencies(ViFrequencyChunk &frequencies)
{
    mTransformer.inverseTransform(frequencies.data(), mSampleChunk.data());
    mTransformer.rescale(mSampleChunk.data());
    write(mSampleChunk);
}

void ViAudioWriteData::writeSplitSamples(ViSampleChunks &samples)
{
    ViSampleChanneler<qreal>::merge(samples, mSampleChunk);
    write(mSampleChunk);
}

void ViAudioWriteData::writeSplitFrequencies(ViFrequencyChunks &frequencies)
{
    ViSampleChanneler<qreal>::merge(frequencies, mFrequencyChunk);
    writeFrequencies(mFrequencyChunk);
}

void ViAudioWriteData::clearOther()
{
}

void ViAudioWriteData::updateOther()
{
    mStream = mBuffer->createWriteStream();
}
