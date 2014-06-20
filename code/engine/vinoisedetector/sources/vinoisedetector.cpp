#include <vinoisedetector.h>
#include <viaudiodata.h>
#include <vinoisecreator.h>

#define WINDOW_SIZE 2048

ViNoiseDetector::ViNoiseDetector()
{
	mOffset = 0;
}

ViNoiseDetector::ViNoiseDetector(const ViNoiseDetector &other)
{
	mOffset = other.mOffset;
	mParameters = other.mParameters;
}

ViNoiseDetector::~ViNoiseDetector()
{
}

void ViNoiseDetector::detect(ViBuffer *corrupted, ViBuffer *noiseMap)
{
	int i, remove;

	noiseMap->clear();
	initialize();

	ViAudioReadData corruptedData(corrupted);
	ViAudioWriteData mapData(noiseMap);
	corruptedData.setSampleCount(WINDOW_SIZE * 2); // times 2 for 2 channels
	mapData.setSampleCount(WINDOW_SIZE * 2);

	ViSampleChunk chunk(WINDOW_SIZE);

	QVector<qreal> map1, map2, samples1, samples2;
	for(i = 0; i < mOffset; ++i)
	{
		map1.append(0);
		map2.append(0);
	}

	while(corruptedData.hasData())
	{
		corruptedData.read();
		ViSampleChunk &corrupted1 = corruptedData.splitSamples(0);
		ViSampleChunk &corrupted2 = corruptedData.splitSamples(1);

		for(i = 0; i < corrupted1.size(); ++i) samples1.append(corrupted1[i]);
		for(i = 0; i < corrupted2.size(); ++i) samples2.append(corrupted2[i]);

		remove = detect(samples1, map1);
		samples1.remove(0, remove);

		remove = detect(samples2, map2);
		samples2.remove(0, remove);

		while(map1.size() >= WINDOW_SIZE)
		{
			for(i = 0; i < WINDOW_SIZE; ++i) chunk[i] = map1[i];
			map1.remove(0, WINDOW_SIZE);
			mapData.enqueueSplitSamples(chunk, 0);
		}

		while(map2.size() >= WINDOW_SIZE)
		{
			for(i = 0; i < WINDOW_SIZE; ++i) chunk[i] = map2[i];
			map2.remove(0, WINDOW_SIZE);
			mapData.enqueueSplitSamples(chunk, 1);
		}
	}

	if(map1.size() > 0)
	{
		chunk.resize(map1.size());
		for(i = 0; i < map1.size(); ++i) chunk[i] = map1[i];
		mapData.enqueueSplitSamples(chunk, 0);
	}

	if(map2.size() > 0)
	{
		chunk.resize(map2.size());
		for(i = 0; i < map2.size(); ++i) chunk[i] = map2[i];
		mapData.enqueueSplitSamples(chunk, 1);
	}
}

void ViNoiseDetector::mask(ViBuffer *noiseMap, ViBuffer *noiseMask, const qreal &threshold)
{
	int i;

	noiseMask->clear();

	ViAudioReadData mapData(noiseMap);
	ViAudioWriteData maskData(noiseMask);
	mapData.setSampleCount(WINDOW_SIZE);
	maskData.setSampleCount(WINDOW_SIZE);

	ViSampleChunk mask(WINDOW_SIZE);

	while(mapData.hasData())
	{
		mapData.read();
		ViSampleChunk &samples = mapData.samples();
		mask.resize(samples.size());

		for(i = 0; i < samples.size(); ++i)
		{
			if(samples[i] >= threshold) mask[i] = 1;
			else mask[i] = 0;
		}

		maskData.write(mask);
	}
}

ViClassificationErrorCollection ViNoiseDetector::error(ViBuffer *noiseMask, ViBuffer *sizeMask)
{
	int i, noiseSize;
	ViClassificationErrorCollection collection;

	ViAudioReadData maskData(noiseMask);
	ViAudioReadData sizeData(sizeMask);
	maskData.setSampleCount(WINDOW_SIZE);
	sizeData.setSampleCount(WINDOW_SIZE);

	while(maskData.hasData())
	{
		maskData.read();
		sizeData.read();
		ViSampleChunk &mask = maskData.samples();
		ViSampleChunk &size = sizeData.samples();

		for(i = 0; i < mask.size(); ++i)
		{
			noiseSize = ViNoiseCreator::fromSizeMask(size[i]);
			collection.at(noiseSize).add(mask[i], noiseSize);
		}
	}

	return collection;
}

ViClassificationErrorCollection ViNoiseDetector::error(ViBuffer *noiseMap, ViBuffer *sizeMask, const qreal &threshold)
{
	int i, noiseSize;
	ViClassificationErrorCollection collection;

	ViAudioReadData mapData(noiseMap);
	ViAudioReadData sizeData(sizeMask);
	mapData.setSampleCount(WINDOW_SIZE);
	sizeData.setSampleCount(WINDOW_SIZE);

	while(mapData.hasData())
	{
		mapData.read();
		sizeData.read();
		ViSampleChunk &map = mapData.samples();
		ViSampleChunk &size = sizeData.samples();

		for(i = 0; i < map.size(); ++i)
		{
			noiseSize = ViNoiseCreator::fromSizeMask(size[i]);
			collection.at(noiseSize).add(map[i] >= threshold, (bool) noiseSize);
		}
	}

	return collection;
}

bool ViNoiseDetector::validParameters() const
{
	return true;
}

void ViNoiseDetector::setParameter(const QString &name, const qreal &value)
{
	if(hasParameter(name))
	{
		mParameters[name] = value;
	}
	else
	{
		LOG("The parameter with the given name does not exist for the detector: " + name, QtFatalMsg);
		exit(-1);
	}
}

void ViNoiseDetector::setParameter(const int &index, const qreal &value)
{
	if(index >= mParameters.size())
	{
		LOG("The parameter at the given index does not exist for the predictor: " + QString::number(index), QtFatalMsg);
		exit(-1);
	}
	else
	{
		mParameters[mParameters.keys()[index]] = value;
	}
}

bool ViNoiseDetector::hasParameter(const QString &name) const
{
	return mParameters.contains(name);
}

int ViNoiseDetector::parameterCount() const
{
	return mParameters.size();
}

QStringList ViNoiseDetector::parameters(const bool &allCaps) const
{
	if(allCaps)
	{
		QStringList parameters;
		QStringList keys = mParameters.keys();
		for(int i = 0; i < keys.size(); ++i) parameters.append(keys[i].toUpper());
		return parameters;
	}
	return mParameters.keys();
}

QString ViNoiseDetector::parameter(const int &index, const bool &allCaps) const
{
	if(index >= mParameters.size()) return "";
	if(allCaps) return mParameters.keys()[index].toUpper();
	return mParameters.keys()[index];
}

qreal ViNoiseDetector::parameter(const QString &name)
{
	if(hasParameter(name))
	{
		LOG("Trying to access an invalid parameter", QtFatalMsg);
		exit(-1);
	}
	return mParameters[name];
}

void ViNoiseDetector::addParameter(const QString &name)
{
	if(name == "")
	{
		LOG("The parameter name cannot be empty", QtFatalMsg);
		exit(-1);
	}
	if(hasParameter(name))
	{
		LOG("The parameter already exists: " + name, QtFatalMsg);
		exit(-1);
	}
	mParameters.insert(name, 0);
}

void ViNoiseDetector::initialize()
{
}

/*#include <viscaler.h>

#define WINDOW_SIZE 8096

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
	mParameterNames = other.mParameterNames;
	initialize(mChannel, 512);
}

ViNoiseDetector::~ViNoiseDetector()
{
	viDeleteAll(mNoise);
	viDeleteAll(mReverseNoise);
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

void ViNoiseDetector::setParameter(const int &number, const qreal &value)
{
	LOG("No parameters implemented for this noise detector.", QtCriticalMsg);
	exit(-1);
}

void ViNoiseDetector::setParameters(const qreal &param1)
{
	LOG("Invalid number of parameters given: 1", QtCriticalMsg);
	exit(-1);
}

void ViNoiseDetector::setParameters(const qreal &param1, const qreal &param2)
{
	LOG("Invalid number of parameters given: 2", QtCriticalMsg);
	exit(-1);
}

void ViNoiseDetector::setParameters(const qreal &param1, const qreal &param2, const qreal &param3)
{
	LOG("Invalid number of parameters given: 3", QtCriticalMsg);
	exit(-1);
}

void ViNoiseDetector::setParameters(const qreal &param1, const qreal &param2, const qreal &param3, const qreal &param4)
{
	LOG("Invalid number of parameters given: 4", QtCriticalMsg);
	exit(-1);
}

bool ViNoiseDetector::validParameters()
{
	return true;
}

bool ViNoiseDetector::hasParameter(const QString &name)
{
	return mParameterNames.contains(name);
}

QString ViNoiseDetector::parameterName(const int &number, const bool &allCaps)
{
	if(number >= mParameterNames.size()) return "";
	if(allCaps) return mParameterNames[number].toUpper();
	return mParameterNames[number];
}

int ViNoiseDetector::parameterCount()
{
	return mParameterNames.size();
}

void ViNoiseDetector::addParameterName(const QString &name)
{
	mParameterNames.append(name);
}

void ViNoiseDetector::setBuffers(ViBuffer *read, ViBuffer *write1, ViBuffer *write2)
{
	clear();

	mRead.setBuffer(read);
	mRead.setSampleCount(WINDOW_SIZE);
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
*/
