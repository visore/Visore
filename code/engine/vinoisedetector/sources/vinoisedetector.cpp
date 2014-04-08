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
	mOffset = 0;
	mThreshold = 0.5;
	mDirection = Forward;
	mAmplification = 1;
}

ViNoiseDetector::ViNoiseDetector(const int &channels, const qint64 samples)
{
	clear();
	initialize(channels, samples);
	mOffset = 0;
	mThreshold = 0.5;
	mDirection = Forward;
	mAmplification = 1;
}

ViNoiseDetector::ViNoiseDetector(const int &channels, const qint64 samples, ViProcessor::ChannelMode mode)
{
	clear();
	initialize(channels, samples);
	setMode(mode);
	mOffset = 0;
	mThreshold = 0.5;
	mDirection = Forward;
	mAmplification = 1;
}

ViNoiseDetector::ViNoiseDetector(const ViNoiseDetector &other)
{
	clear();
	mMode = other.mMode;
	mChannel = other.mChannel;
	mIndexes = other.mIndexes;
	mReverseIndexes = other.mReverseIndexes;
	mOffset = other.mOffset;
	mThreshold = other.mThreshold;
	mDirection = other.mDirection;
	mAmplification = other.mAmplification;
}

ViNoiseDetector::~ViNoiseDetector()
{
}

void ViNoiseDetector::setDirection(Direction direction)
{
	mDirection = direction;
}

ViNoiseDetector::Direction ViNoiseDetector::direction()
{
	return mDirection;
}

void ViNoiseDetector::setOffset(const int &offset)
{
	mOffset = offset;
}

void ViNoiseDetector::initialize(const int &channels, const qint64 samples)
{
	viDeleteAll(mNoise);
	viDeleteAll(mReverseNoise);
	qint64 size = qCeil(samples / qreal(channels));
	for(int i = 0; i < channels; ++i)
	{
		mIndexes.append(0);
		mReverseIndexes.append(0);

		ViNoise *noise = new ViNoise(size);
		noise->setThreshold(mThreshold);
		mNoise.append(noise);

		ViNoise *noise2 = new ViNoise(size);
		noise2->setThreshold(mThreshold);
		mReverseNoise.append(noise2);
	}
}

void ViNoiseDetector::setThreshold(const qreal &threshold)
{
	mThreshold = threshold;
	for(int i = 0; i < mNoise.size(); ++i)
	{
		mNoise[i]->setThreshold(threshold);
		mReverseNoise[i]->setThreshold(threshold);
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
	/*if(mMode == ViProcessor::Separated)
	{
		calculateNoise(mData->scaledSplitSamples(mChannel, 0, 1));
	}
	else
	{
		calculateNoise(mData->scaledSamples(0, 1));
	}*/

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
	if(mReverse)
	{
		mReverseNoise[mChannel]->set(mReverseIndexes[mChannel], value * mAmplification);
		mReverseIndexes[mChannel] += 1;
	}
	else
	{
		mNoise[mChannel]->set(mIndexes[mChannel], value * mAmplification);
		mIndexes[mChannel] += 1;
	}
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
	mReverseIndexes.clear();

	viDeleteAll(mNoise);
	viDeleteAll(mReverseNoise);

	mCache.clear();

	mRead.clear();
	mWrite1.clear();
	mWrite2.clear();
}

void ViNoiseDetector::setAmplification(const qreal &amp)
{
	mAmplification = amp;
}

qreal ViNoiseDetector::amplification()
{
	return mAmplification;
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
	mReverse = false;
	qint64 totalSamples = mRead.bufferSamples();
	qint64 channelSamples = totalSamples / mRead.channelCount();
	qint64 newSize, i, j;
	int c;

    qint64 totalSize = mRead.bufferSamples(), processedSize = 0;
    if(mDirection == Bidirectional) totalSize *= 2;

	initialize(mRead.channelCount(), totalSamples);
	for(i = 0; i < mRead.channelCount(); ++i)
	{
		mCache.append(QQueue<qreal>());
	}

	if(mDirection & Forward)
	{
		for(i = 0; i < mRead.channelCount(); ++i)
		{
			setChannel(i);
			for(j = 0; j < mOffset; ++j)
			{
				setNoise(0);
			}
		}

		while(mRead.hasData())
		{
			mRead.read();

			for(i = 0; i < mRead.channelCount(); ++i)
			{
				setChannel(i);
				const ViSampleChunk &samples = mRead.splitSamples(i);
				for(j = 0; j < samples.size(); ++j)
				{
					mCache[i].append(samples[j]);
				}
				calculateNoise(mCache[i]);
			}

            processedSize += mRead.sampleCount();
            setProgress((processedSize * 99.0) / totalSize);
		}

		for(c = 0; c < mRead.channelCount(); ++c)
		{
			setChannel(c);
			newSize = mNoise[c]->size();
			while(newSize < channelSamples) setNoise(0);
		}
	}

	if(mDirection & Reversed)
	{
		mRead.clear();
		mCache.clear();
		for(i = 0; i < mRead.channelCount(); ++i)
		{
			mCache.append(QQueue<qreal>());
		}

		mReverse = true;
		mRead.setReversed();
		int end;

		for(i = 0; i < mRead.channelCount(); ++i)
		{
			setChannel(i);
			end = mOffset;
			if(i == 0) --end; // For some reason the 1st channel is ahead of 1 sample when processing in reversed
			for(j = 0; j < end; ++j)
			{
				setNoise(0);
			}
		}

		while(mRead.hasData())
		{
			mRead.read();

			for(i = 0; i < mRead.channelCount(); ++i)
			{
				setChannel(i);
				const ViSampleChunk &samples = mRead.splitSamples(i);
				for(j = 0; j < samples.size(); ++j)
				{
					mCache[i].append(samples[j]);
				}
				calculateNoise(mCache[i]);
			}

            processedSize += mRead.sampleCount();
            setProgress((processedSize * 99.0) / totalSize);
		}
		for(c = 0; c < mRead.channelCount(); ++c)
		{
			setChannel(c);
			newSize = mReverseNoise[c]->size();
			while(newSize < channelSamples) setNoise(0);
		}
	}

	if(mDirection == Bidirectional)
	{
		for(c = 0; c < mRead.channelCount(); ++c)
		{
			newSize = mReverseNoise[c]->size();
			for(i = 0; i < newSize; ++i)
			{
				mNoise[c]->set(i, (mNoise[c]->get(i) + mReverseNoise[c]->get(newSize - i - 1)) / 2);
			}
		}
	}
	else if(mDirection == Reversed)
	{
		for(c = 0; c < mRead.channelCount(); ++c)
		{
			newSize = mReverseNoise[c]->size();
			for(i = 0; i < newSize; ++i)
			{
				mNoise[c]->set(i, mReverseNoise[c]->get(newSize - i - 1));
			}
		}
	}

	viDeleteAll(mReverseNoise);

	for(i = 0; i < mRead.channelCount(); ++i)
	{
		mNoise[i]->generateMask();
	}

	int size = mWrite1.sampleCount() / 2;
	qint64 offset = 0;
	ViSampleChunk chunk(size);

	while(offset < channelSamples)
	{
		for(i = 0; i < mRead.channelCount(); ++i)
		{
			chunk = mNoise[i]->data()->subset(offset, size);
			mWrite1.enqueueSplitSamples(chunk,i);

			chunk = mNoise[i]->mask()->subset(offset, size);
			mWrite2.enqueueSplitSamples(chunk,i);
		}
		offset += size;
	}

	mCache.clear();
	//clear();
	setProgress(100);
	emit finished();
}
