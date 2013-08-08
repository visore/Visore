#include <vinoisedetector.h>

ViNoiseDetector::ViNoiseDetector()
{
    clear();
}

ViNoiseDetector::ViNoiseDetector(ViProcessor::ChannelMode mode)
{
	clear();
	setMode(mMode);
}

ViNoiseDetector::ViNoiseDetector(const ViNoiseDetector &other)
{
	mMode = other.mMode;
	mChannel = other.mChannel;
	mData = other.mData;
    mNoise = other.mNoise;
}

ViNoiseDetector::~ViNoiseDetector()
{
}

void ViNoiseDetector::setMode(ViProcessor::ChannelMode mode)
{
	mMode = mode;
}

ViProcessor::ChannelMode ViNoiseDetector::mode()
{
	return mMode;
}

void ViNoiseDetector::setChannel(int channel)
{
	mChannel = channel;
}

int ViNoiseDetector::channel()
{
	return mChannel;
}

void ViNoiseDetector::setData(ViAudioReadData &data)
{
	mData = &data;
}

bool ViNoiseDetector::hasData()
{
	return mData != NULL;
}

bool ViNoiseDetector::isNoisy(int channel)
{
	setChannel(channel);
	return isNoisy();
}

bool ViNoiseDetector::isNoisy(ViAudioReadData &data)
{
	setData(data);
	return isNoisy();
}

bool ViNoiseDetector::isNoisy(ViAudioReadData &data, int channel)
{
	setData(data);
	setChannel(channel);
	return isNoisy();
}

bool ViNoiseDetector::isNoisy()
{
	if(mMode == ViProcessor::Separated)
	{
		ViSampleChunk &samples = mData->splitSamples(mChannel);
		mNoise.resize(samples.size());
		calculateNoise(mNoise, samples);
	}
	else
	{
		ViSampleChunk &samples = mData->samples();
		mNoise.resize(samples.size());
		calculateNoise(mNoise, samples);
	}

	mNoise.minimize();
	return mNoise.isNoisy();
}

ViNoise& ViNoiseDetector::noise()
{
	return mNoise;
}

void ViNoiseDetector::clear()
{
	mData = NULL;
	mChannel = 0;
	setMode(ViProcessor::Combined);
    mNoise.clear();
}
