#include <viinterpolator.h>
#include <viaudiodata.h>
#include <vinoisecreator.h>
#include <vilogger.h>
#include <QtMath>

#define DEFAULT_WINDOW_SIZE 0	// Will use all available samples
#define FALLBACK_SIZE 8192	// In case the window size is 0 (use all samples), use this to regulate the caches
#define MAX_WINDOW_SIZE 64768	// Even if the window size is 0 (use all data), the window size will never be larger than this

ViInterpolator::ViInterpolator()
	: ViLibrary()
{
	setWindowSize(DEFAULT_WINDOW_SIZE);
}

ViInterpolator::ViInterpolator(const ViInterpolator &other)
	: ViLibrary(other)
{
	mHalfWindowSize = other.mHalfWindowSize;
	mHalfWindowSizeLeft = other.mHalfWindowSizeLeft;
	mHalfWindowSizeRight = other.mHalfWindowSizeRight;
	mWindowSize = other.mWindowSize;
	mParameterNames = other.mParameterNames;
}

ViInterpolator::~ViInterpolator()
{
}

void ViInterpolator::setWindowSize(const int &size)
{
	mHalfWindowSize = floor(size / 2.0);

	mWindowSize = size;
	mHalfWindowSizeLeft = ceil(size / 2.0);
	mHalfWindowSizeRight = floor(size / 2.0);
}

int ViInterpolator::windowSize()
{
	return mWindowSize;
}

void ViInterpolator::setParameter(const int &number, const qreal &value)
{
	LOG("No parameters implemented for this interpolator.", QtCriticalMsg);
	exit(-1);
}

void ViInterpolator::setParameter(const QString &name, const qreal &value)
{
	QString newName = name.trimmed().toLower().replace(" ", "");
	for(int i = 0; i < mParameterNames.size(); ++i)
	{
		QString currentName = mParameterNames[i].trimmed().toLower().replace(" ", "");
		if(newName == currentName)
		{
			setParameter(i, value);
			return;
		}
	}
}

void ViInterpolator::setParameters(const qreal &parameter1)
{
	if(mParameterNames.size() < 1)
	{
		LOG("Invalid parameter count for this interpolator.", QtCriticalMsg);
		exit(-1);
	}
	setParameter(0, parameter1);
}

void ViInterpolator::setParameters(const qreal &parameter1, const qreal &parameter2)
{
	if(mParameterNames.size() < 2)
	{
		LOG("Invalid parameter count for this interpolator.", QtCriticalMsg);
		exit(-1);
	}
	setParameter(0, parameter1);
	setParameter(1, parameter2);
}

void ViInterpolator::setParameters(const qreal &parameter1, const qreal &parameter2, const qreal &parameter3)
{
	if(mParameterNames.size() < 3)
	{
		LOG("Invalid parameter count for this interpolator.", QtCriticalMsg);
		exit(-1);
	}
	setParameter(0, parameter1);
	setParameter(1, parameter2);
	setParameter(2, parameter3);
}

void ViInterpolator::setParameters(const qreal &parameter1, const qreal &parameter2, const qreal &parameter3, const qreal &parameter4)
{
	if(mParameterNames.size() < 4)
	{
		LOG("Invalid parameter count for this interpolator.", QtCriticalMsg);
		exit(-1);
	}
	setParameter(0, parameter1);
	setParameter(1, parameter2);
	setParameter(2, parameter3);
	setParameter(3, parameter4);
}

void ViInterpolator::setParameters(const qreal &parameter1, const qreal &parameter2, const qreal &parameter3, const qreal &parameter4, const qreal &parameter5)
{
	if(mParameterNames.size() < 5)
	{
		LOG("Invalid parameter count for this interpolator.", QtCriticalMsg);
		exit(-1);
	}
	setParameter(0, parameter1);
	setParameter(1, parameter2);
	setParameter(2, parameter3);
	setParameter(3, parameter4);
	setParameter(4, parameter5);
}

bool ViInterpolator::validParameters()
{
	return true;
}

bool ViInterpolator::hasParameter(const QString &name)
{
	return mParameterNames.contains(name);
}

QString ViInterpolator::parameterName(const int &index, const bool &allCaps)
{
	if(index >= mParameterNames.size()) return "";
	if(allCaps) return mParameterNames[index].toUpper();
	return mParameterNames[index];
}

void ViInterpolator::addParameterName(const QString &name)
{
	mParameterNames.append(name);
}

void ViInterpolator::adjustSamples(ViSampleChunk &data)
{
	static int i;
	for(i = 0; i < data.size(); ++i)
	{
		if(data[i] > 1) data[i] = 1;
		else if(data[i] < -1) data[i] = -1;
	}
}

bool ViInterpolator::interpolate(ViBuffer *input, ViBuffer *output, ViBuffer *mask, ViErrorCollection *modelErrors)
{
	modelErrors->clear();

	ViAudioReadData corrupted(input);
	ViAudioReadData noise(mask);
	ViAudioWriteData corrected(output);
	corrupted.setSampleCount(8192*2);
	noise.setSampleCount(8192*2);
	corrected.setSampleCount(8192*2);

	ViSampleChunk output1, output2;
	QVector<qreal> leftCache1, leftCache2, rightCache1, rightCache2, noiseCache1, noiseCache2;
	int noiseSize1 = 0, noiseSize2 = 0;

	qint64 processedSize = 0, totalSize = corrupted.bufferSamples();

	while(corrupted.hasData())
	{
		corrupted.read();
		ViSampleChunk &corrupted1 = corrupted.splitSamples(0);
		ViSampleChunk &corrupted2 = corrupted.splitSamples(1);

		noise.read();
		ViSampleChunk &noise1 = noise.splitSamples(0);
		ViSampleChunk &noise2 = noise.splitSamples(1);

		process(corrupted1, output1, noise1, noiseSize1, noiseCache1, leftCache1, rightCache1, modelErrors);
		process(corrupted2, output2, noise2, noiseSize2, noiseCache2, leftCache2, rightCache2, modelErrors);

		adjustSamples(output1);
		adjustSamples(output2);

		corrected.enqueueSplitSamples(output1, 0);
		corrected.enqueueSplitSamples(output2, 1);

		output1.clear();
		output2.clear();

		processedSize += corrupted1.size() + corrupted2.size();
		setProgress((processedSize * 99.0) / totalSize);
	}

	// Interpolate the remaining samples
	interpolate(output1, noiseSize1, noiseCache1, leftCache1, rightCache1, modelErrors);
	interpolate(output2, noiseSize2, noiseCache2, leftCache2, rightCache2, modelErrors);
	adjustSamples(output1);
	adjustSamples(output2);
	corrected.enqueueSplitSamples(output1, 0);
	corrected.enqueueSplitSamples(output2, 1);

	// Write the remaining right cache
	output1.resize(rightCache1.size());
	for(int i = 0; i < output1.size(); ++i) output1[i] = rightCache1[i];
	corrected.enqueueSplitSamples(output1, 0);

	output2.resize(rightCache2.size());
	for(int i = 0; i < output2.size(); ++i) output2[i] = rightCache2[i];
	corrected.enqueueSplitSamples(output2, 1);

	setProgress(100);
}

bool ViInterpolator::interpolate(ViBuffer *input, ViBuffer *output, ViBuffer *target, ViBuffer *sizeMask, ViErrorCollection *gapErrors, ViErrorCollection *modelErrors)
{
	gapErrors->clear();
	modelErrors->clear();

	interpolate(input, output, sizeMask, modelErrors);

	ViAudioReadData readCorrected(output);
	ViAudioReadData readTarget(target);
	ViAudioReadData readMask(sizeMask);
	readCorrected.setSampleCount(8192*2);
	readTarget.setSampleCount(8192*2);
	readMask.setSampleCount(8192*2);

	int i, size, noiseSize;

	while(readCorrected.hasData())
	{
		readCorrected.read();
		ViSampleChunk &corrected1 = readCorrected.splitSamples(0);
		ViSampleChunk &corrected2 = readCorrected.splitSamples(1);

		readTarget.read();
		ViSampleChunk &target1 = readTarget.splitSamples(0);
		ViSampleChunk &target2 = readTarget.splitSamples(1);

		readMask.read();
		ViSampleChunk &noise1 = readMask.splitSamples(0);
		ViSampleChunk &noise2 = readMask.splitSamples(1);

		size = corrected1.size();
		for(i = 0; i < size; ++i)
		{
			noiseSize = ViNoiseCreator::fromSizeMask(noise1[i]);
			if(noiseSize > 0) gapErrors->at(noiseSize).add(corrected1[i], target1[i]);
		}

		size = corrected2.size();
		for(i = 0; i < size; ++i)
		{
			noiseSize = ViNoiseCreator::fromSizeMask(noise2[i]);
			if(noiseSize > 0) gapErrors->at(noiseSize).add(corrected2[i], target2[i]);
		}
	}
}

void ViInterpolator::process(const ViSampleChunk &input, ViSampleChunk &output, const ViSampleChunk &noise, int &noiseSize, QVector<qreal> &noiseCache, QVector<qreal> &leftCache, QVector<qreal> &rightCache, ViErrorCollection *modelErrors)
{
	int tempNoiseSize = 0;
	for(int i = 0; i < input.size(); ++i)
	{
		if(noise[i])
		{
			if(tempNoiseSize == 0 && noiseSize != 0)
			{
				interpolate(output, noiseSize, noiseCache, leftCache, rightCache, modelErrors);
				noiseSize = 0;
				leftCache = rightCache;
				rightCache.clear();
				noiseCache.clear();
			}
			++tempNoiseSize;
			noiseCache.append(input[i]);
		}
		else
		{
			if(tempNoiseSize > 0)
			{
				noiseSize = tempNoiseSize;
				tempNoiseSize = 0;
			}
			if(noiseSize > 0) rightCache.append(input[i]);
			else
			{
				leftCache.append(input[i]);
				if(mWindowSize == 0 && leftCache.size() >= FALLBACK_SIZE)
				{
					int halfFallback = FALLBACK_SIZE / 2;
					ViSampleChunk outputData(halfFallback);
					for(int i = 0; i < halfFallback; ++i) outputData[i] = leftCache[i];
					leftCache.remove(0, halfFallback);
					output.append(outputData);
				}
				else if(leftCache.size() >= mWindowSize)
				{
					ViSampleChunk outputData(mHalfWindowSize);
					for(int i = 0; i < mHalfWindowSize; ++i) outputData[i] = leftCache[i];
					leftCache.remove(0, mHalfWindowSize);
					output.append(outputData);
				}
			}
		}
	}
}

void ViInterpolator::interpolate(ViSampleChunk &output, const int &noiseSize, QVector<qreal> &noiseCache, QVector<qreal> &leftCache, QVector<qreal> &rightCache, ViErrorCollection *modelErrors)
{
	int i;
	ViSampleChunk outputData(noiseSize + leftCache.size());
	for(i = 0; i < leftCache.size(); ++i) outputData[i] = leftCache[i];
	if(noiseSize == 0)
	{
		output.append(outputData);
		return;
	}
	for(i = 0; i < noiseCache.size(); ++i) outputData[i + leftCache.size()] = noiseCache[i];

	int leftSize = leftCache.size();
	int rightSize = rightCache.size();

	int ls = leftSize;

	qreal *leftData = leftCache.data();
	qreal *rightData = rightCache.data();

	if(mHalfWindowSize > 0)
	{
		if(leftSize > mHalfWindowSizeLeft) leftData += (leftSize - mHalfWindowSizeLeft);
		leftSize = qMin(leftSize, mHalfWindowSizeLeft);
		rightSize = qMin(rightSize, mHalfWindowSizeRight);
	}

	if(modelErrors == NULL) interpolate(leftData, leftSize, rightData, rightSize, outputData.data() + leftCache.size(), noiseSize, NULL);
	else interpolate(leftData, leftSize, rightData, rightSize, outputData.data() + leftCache.size(), noiseSize, &modelErrors->at(noiseSize));
	output.append(outputData);
}

ViElement ViInterpolator::exportData()
{
	ViElement root("interpolator");
	root.addChild("name", name());
	return root;
}

bool ViInterpolator::importData(ViElement element)
{
	return element.name() == "interpolator" && element.child("name").toString() == name();
}

ViDegreeInterpolator::ViDegreeInterpolator()
	: ViInterpolator()
{
	setDegree(1);
}

ViDegreeInterpolator::ViDegreeInterpolator(const int &degree)
	: ViInterpolator()
{
	setDegree(degree);
}

ViDegreeInterpolator::ViDegreeInterpolator(const ViDegreeInterpolator &other)
	: ViInterpolator(other)
{
	mDegree = other.mDegree;
}

ViDegreeInterpolator::~ViDegreeInterpolator()
{
}

void ViDegreeInterpolator::setDegree(const int &degree)
{
	if(degree < 1)
	{
		LOG("Invalid degree " + QString::number(degree) + " specified. Setting degree to 1.", QtCriticalMsg);
		mDegree = 1;
	}
	else
	{
		mDegree = degree;
	}
}

int ViDegreeInterpolator::degree() const
{
	return mDegree;
}
