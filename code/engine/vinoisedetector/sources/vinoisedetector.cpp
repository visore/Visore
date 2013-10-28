#include <vinoisedetector.h>

#include <viscaler.h>

void ViNoiseDetectorThread::setDetector(ViNoiseDetector *detector)
{
	mDetector = detector;
}

void ViNoiseDetectorThread::run()
{
	mDetector->create();
}

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

	mRead.clear();
	mWrite1.clear();
	mWrite2.clear();
}

void ViNoiseDetector::setBuffers(ViBuffer *read, ViBuffer *write1, ViBuffer *write2)
{
	clear();

	mRead.setBuffer(read);
	mWrite1.setBuffer(write1);
	mWrite2.setBuffer(write2);
}

void ViNoiseDetector::generate()
{
	mThread.setDetector(this);
	mThread.start();
}

void ViNoiseDetector::create()
{
	qint64 totalSamples = mRead.bufferSamples();
	qint64 currentSamples = 0;
	int i;

	initialize(mRead.channelCount(), totalSamples);

	while(mRead.hasData())
	{
		mRead.read();
		currentSamples += mRead.sampleCount();

		for(i = 0; i < mRead.channelCount(); ++i)
		{
			setChannel(i);
			const ViSampleChunk &samples = mRead.splitSamples(i);
			calculateNoise(samples);
		}
		setProgress((currentSamples * 97.0) / totalSamples);
	}

	for(i = 0; i < mRead.channelCount(); ++i)
	{
		mNoise[i]->generateMask();
	}

	int size = mWrite1.sampleCount() / 2;
	qint64 offset = 0;
	ViSampleChunk chunk(size);
	ViSampleChunk &s1  =*mNoise[0]->data();
	ViSampleChunk &s2  =*mNoise[1]->data();
	ViSampleChunk &m1  =*mNoise[0]->mask();
	ViSampleChunk &m2  =*mNoise[1]->mask();

	while(offset < (totalSamples / 2))
	{
		for(int j = 0; j < size;++j)chunk[j]=s1[j+offset];
		mWrite1.enqueueSplitSamples(chunk,0);

		for(int j = 0; j < size;++j)chunk[j]=s2[j+offset];
		mWrite1.enqueueSplitSamples(chunk,1);

		for(int j = 0; j < size;++j)chunk[j]=m1[j+offset];
		mWrite2.enqueueSplitSamples(chunk,0);

		for(int j = 0; j < size;++j)chunk[j]=m2[j+offset];
		mWrite2.enqueueSplitSamples(chunk,1);

		/*for(i = 0; i < mRead.channelCount(); ++i)
		{
			chunk = mNoise[i]->data()->subset(offset, size);
			mWrite1.enqueueSplitSamples(chunk, i);
		}*/
		offset += size;
	}

	/*
	for(i = 0; i < mRead.channelCount(); ++i)
	{
		mNoise[i]->generateMask();
	}
	offset = 0;
	while(offset < (totalSamples / 2))
	{
		for(i = 0; i < mRead.channelCount(); ++i)
		{
			chunk = mNoise[i]->mask()->subset(offset, size);
			mWrite2.enqueueSplitSamples(chunk, i);
		}
		offset += size;
	}*/

	//clear();
	setProgress(100);
	emit finished();
}
