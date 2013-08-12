#include <vinoisedetector.h>

#include <viscaler.h>

#define DEFAULT_WINDOW_SIZE 512

ViNoiseDetector::ViNoiseDetector(const int &windowSize)
{
    clear();
	setWindowSize(windowSize);
}

ViNoiseDetector::ViNoiseDetector(ViProcessor::ChannelMode mode, const int &windowSize)
{
	clear();
	setMode(mMode);
	setWindowSize(windowSize);
}

ViNoiseDetector::ViNoiseDetector(const ViNoiseDetector &other)
{
	mWindowSize = other.mWindowSize;
	mMode = other.mMode;
	mChannel = other.mChannel;
	mData = other.mData;
    mNoise = other.mNoise;
}

ViNoiseDetector::~ViNoiseDetector()
{
}

void ViNoiseDetector::setWindowSize(const int &size)
{
	if(size < 0) mWindowSize = DEFAULT_WINDOW_SIZE;
	else mWindowSize = size;
}

int ViNoiseDetector::windowSize() const
{
	return mWindowSize;
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
	/*if(mMode == ViProcessor::Separated)
	{
		calculateNoise(mData->scaledSplitSamples(mChannel, 0, 1));
	}
	else
	{
		calculateNoise(mData->scaledSamples(0, 1));
	}*/

	ViSampleChunk c1;
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
	calculateNoise(c1);

	//mNoise.minimize();
	return mNoise.isNoisy();
}

void ViNoiseDetector::calculateNoise(const ViSampleChunk &samples)
{
	mNoise.resize(samples.size());
	QList<ViSampleChunk> chunks = samples.subsets(mWindowSize);
	ViNoise noise;
	for(int i = 0; i < chunks.size(); ++i)
	{
		noise.resize(chunks[i].size());
		calculateNoise(noise, chunks[i]);
		mNoise.set(i * mWindowSize, noise);
	}
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
