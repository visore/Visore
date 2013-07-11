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
    mCounter = other.mCounter;
    mNoise = other.mNoise;
}

ViNoiseDetector::~ViNoiseDetector()
{
}

void ViNoiseDetector::setFormat(const ViAudioFormat &format)
{
	mNoise.setFormat(format);
}

void ViNoiseDetector::setMode(ViProcessor::ChannelMode mode)
{
	mMode = mode;
	if(mMode == ViProcessor::Separated)
	{
		mNoise.setSeparateChannels();
	}
	else
	{
		mNoise.setCombinedChannels();
	}
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
	int size = mData->sampleCount();
	bool returnValue = false;

	if(calculateNoise())
	{
		mNoise.add(mCounter, size, mChannel);

		QString message = "Noise detected (sample " + QString::number(mCounter);
		if(mMode == ViProcessor::Separated)
		{
			message += ", sample " + QString::number(mCounter / mNoise.format().channelCount()) + " in channel " + QString::number(mChannel + 1);
		}
		message += ").";
		LOG(message);

		returnValue = true;
	}

	if(mMode == ViProcessor::Combined || mChannel == mNoise.format().channelCount() - 1)
	{
		mCounter += size;
	}

	return returnValue;
}

ViNoiseList ViNoiseDetector::noisyWindows()
{
    return mNoise;
}

void ViNoiseDetector::clear()
{
	mData = NULL;
	mChannel = 0;
	setMode(ViProcessor::Combined);
    mCounter = 0;
    mNoise.clear();
}

ViElement ViNoiseDetector::exportData()
{
    /*ViElement root("noisedetector");
    root.addChild("name", name());
    root.addChild("windowsize", mProcessor->sampleCount());
    ViElement noise("noisywindows");
    for(int i = 0; i < mNoise.size(); ++i)
    {
        noise.addChild("window", mNoise[i]);
    }
    root.addChild(noise);
    return root;*/
}

bool ViNoiseDetector::importData(ViElement element)
{
    /*if(element.name() != "noisedetector")
    {
        element = element.child("noisedetector");
        if(element.name() != "noisedetector")
        {
            return false;
        }
    }
    if(!element.hasChild("noisywindows"))
    {
        return false;
    }

    clear();
    ViElementList windows = element.child("noisywindows").children("window");
    for(int i = 0; i < windows.size(); ++i)
    {
        mNoise.append(windows[i].toInt());
    }

    return true;*/
}

ViSampleChunk& ViNoiseDetector::samples()
{
	if(mMode == ViProcessor::Separated)
	{
		return mData->splitSamples(mChannel);
	}
	return mData->samples();
}

ViFrequencyChunk& ViNoiseDetector::frequencies()
{
	if(mMode == ViProcessor::Separated)
	{
		return mData->splitFrequencies(mChannel);
	}
	return mData->frequencies();
}
