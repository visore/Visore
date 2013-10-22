#include <vinoisedetector.h>

#include <viscaler.h>

ViNoiseDetector::ViNoiseDetector()
{
	clear();
}

ViNoiseDetector::ViNoiseDetector(const int &channels, const qint64 samples)
{
	clear();
	initialize(channels, samples);
}

ViNoiseDetector::ViNoiseDetector(const int &channels, const qint64 samples, ViProcessor::ChannelMode mode)
{
	clear();
	initialize(channels, samples);
	setMode(mode);
}

ViNoiseDetector::ViNoiseDetector(const ViNoiseDetector &other)
{
	mMode = other.mMode;
	mChannel = other.mChannel;
	mData = other.mData;
    mNoise = other.mNoise;
	mIndexes = other.mIndexes;
}

ViNoiseDetector::~ViNoiseDetector()
{
}

void ViNoiseDetector::initialize(const int &channels, const qint64 samples)
{
	qint64 size = qCeil(samples / qreal(channels));
	for(int i = 0; i < channels; ++i)
	{
		mIndexes.append(0);
		mNoise.append(new ViNoise(size));
	}
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
		calculateNoise(mData->scaledSplitSamples(mChannel, 0, 1));
	}
	else
	{
		calculateNoise(mData->scaledSamples(0, 1));
	}

	/*ViSampleChunk c1;
	if(mMode == ViProcessor::Separated)
	{
		c1 = mData->splitSamples(mChannel);
	}
	else
	{
		c1 = mData->samples();
	}
	for(int i = 0; i < c1.size();++i)
	{
		//c1[i] = ViScaler<qreal>::scale(c1[i], -1, 1, 0, 1);
	}
	calculateNoise(c1);*/

	//mNoise[mChannel].minimize();
	//return mNoise[mChannel]isNoisy();
}

void ViNoiseDetector::setNoise(const qreal &value)
{
	mNoise[mChannel]->set(mIndexes[mChannel], value);
	mIndexes[mChannel] += 1;
}

ViNoise& ViNoiseDetector::noise(const int &channel)
{
	return *mNoise[channel];
}

void ViNoiseDetector::clear()
{
	mData = NULL;
	mChannel = 0;
	setMode(ViProcessor::Combined);
	mIndexes.clear();
	viDeleteAll(mNoise);
}
