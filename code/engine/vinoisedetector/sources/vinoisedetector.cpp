#include <vinoisedetector.h>
#include <viaudiodata.h>
#include <vinoisecreator.h>

#define WINDOW_SIZE 2048

ViNoiseDetector::ViNoiseDetector()
{
	mOffset = 0;
	mScale = 1;
}

ViNoiseDetector::ViNoiseDetector(const ViNoiseDetector &other)
{
	mOffset = other.mOffset;
	mScale = other.mScale;
	mParameters = other.mParameters;
}

ViNoiseDetector::~ViNoiseDetector()
{
}

qreal ViNoiseDetector::detect(ViBuffer *corrupted, ViBuffer *noiseMap)
{
	int i;
	qreal maxNoise = 0;

	noiseMap->clear();
	initialize();
	setProgress(0);

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

	qint64 totalSamples = corruptedData.bufferSamples();
	qint64 processedSamples = 0;

	while(corruptedData.hasData())
	{
		corruptedData.read();
		ViSampleChunk &corrupted1 = corruptedData.splitSamples(0);
		ViSampleChunk &corrupted2 = corruptedData.splitSamples(1);

		for(i = 0; i < corrupted1.size(); ++i) samples1.append(corrupted1[i]);
		for(i = 0; i < corrupted2.size(); ++i) samples2.append(corrupted2[i]);

		detect(samples1, map1);
		detect(samples2, map2);

		while(map1.size() >= WINDOW_SIZE)
		{
			for(i = 0; i < WINDOW_SIZE; ++i)
			{
				chunk[i] = map1[i] / mScale;
				if(chunk[i] > maxNoise) maxNoise = chunk[i];
			}
			map1.remove(0, WINDOW_SIZE);
			mapData.enqueueSplitSamples(chunk, 0);
		}

		while(map2.size() >= WINDOW_SIZE)
		{
			for(i = 0; i < WINDOW_SIZE; ++i)
			{
				chunk[i] = map2[i] / mScale;
				if(chunk[i] > maxNoise) maxNoise = chunk[i];
			}
			map2.remove(0, WINDOW_SIZE);
			mapData.enqueueSplitSamples(chunk, 1);
		}

		processedSamples += corrupted1.size() + corrupted2.size();
		setProgress((processedSamples * 99.0) / totalSamples);
	}

	if(map1.size() > 0)
	{
		chunk.resize(map1.size());
		for(i = 0; i < map1.size(); ++i)
		{
			chunk[i] = map1[i] / mScale;
			if(chunk[i] > maxNoise) maxNoise = chunk[i];
		}
		mapData.enqueueSplitSamples(chunk, 0);
	}

	if(map2.size() > 0)
	{
		chunk.resize(map2.size());
		for(i = 0; i < map2.size(); ++i)
		{
			chunk[i] = map2[i] / mScale;
			if(chunk[i] > maxNoise) maxNoise = chunk[i];
		}
		mapData.enqueueSplitSamples(chunk, 1);
	}

	setProgress(100);
	return maxNoise;
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
		return mParameters[name];
	}
	LOG("Trying to access an invalid parameter: " + name, QtFatalMsg);
	exit(-1);
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

void ViNoiseDetector::setOffset(const int &offset)
{
	mOffset = offset;
}

qreal ViNoiseDetector::unscaleThreshold(const qreal &threshold)
{
	return threshold * mScale;
}

void ViNoiseDetector::setScale(const qreal &scale)
{
	mScale = scale;
}

void ViNoiseDetector::initialize()
{
}
