#include <viinterpolator.h>
#include <viaudiodata.h>
#include <vilogger.h>

#define DEFAULT_WINDOW_SIZE 0	// Will use all available samples
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
	mWindowSize = other.mWindowSize;
}

ViInterpolator::~ViInterpolator()
{
}

void ViInterpolator::setWindowSize(const int &size)
{
	mHalfWindowSize = qFloor(size / 2);
	mWindowSize = mHalfWindowSize * 2;
	if(mHalfWindowSize == 0) setWindowSize(MAX_WINDOW_SIZE);
}

int ViInterpolator::windowSize()
{
	return mWindowSize;
}

bool ViInterpolator::interpolate(ViBuffer *input, ViBuffer *output, ViBuffer *mask)
{
	ViAudioReadData corrupted(input);
	ViAudioReadData noise(mask);
	ViAudioWriteData corrected(output);
	corrupted.setSampleCount(8192*2);
	noise.setSampleCount(8192*2);
	corrected.setSampleCount(8192*2);

	ViSampleChunk output1, output2;
	QVector<qreal> leftCache1, leftCache2, rightCache1, rightCache2, noiseCache1, noiseCache2;
	int noiseSize1 = 0, noiseSize2 = 0;

	while(corrupted.hasData())
	{
		corrupted.read();
		ViSampleChunk &corrupted1 = corrupted.splitSamples(0);
		ViSampleChunk &corrupted2 = corrupted.splitSamples(1);

		noise.read();
		ViSampleChunk &noise1 = noise.splitSamples(0);
		ViSampleChunk &noise2 = noise.splitSamples(1);

		process(corrupted1, output1, noise1, noiseSize1, noiseCache1, leftCache1, rightCache1);
		process(corrupted2, output2, noise2, noiseSize2, noiseCache2, leftCache2, rightCache2);

		corrected.enqueueSplitSamples(output1, 0);
		corrected.enqueueSplitSamples(output2, 1);

		output1.clear();
		output2.clear();
	}

	// Interpolate the remaining samples
	interpolate(output1, noiseSize1, noiseCache1, leftCache1, rightCache1);
	interpolate(output2, noiseSize2, noiseCache2, leftCache2, rightCache2);
	corrected.enqueueSplitSamples(output1, 0);
	corrected.enqueueSplitSamples(output2, 1);

	// Write the remaining right cache
	output1.resize(rightCache1.size());
	for(int i = 0; i < output1.size(); ++i) output1[i] = rightCache1[i];
	corrected.enqueueSplitSamples(output1, 0);

	output2.resize(rightCache2.size());
	for(int i = 0; i < output2.size(); ++i) output2[i] = rightCache2[i];
	corrected.enqueueSplitSamples(output2, 1);
}

void ViInterpolator::process(const ViSampleChunk &input, ViSampleChunk &output, const ViSampleChunk &noise, int &noiseSize, QVector<qreal> &noiseCache, QVector<qreal> &leftCache, QVector<qreal> &rightCache)
{
	int tempNoiseSize = 0;
	for(int i = 0; i < input.size(); ++i)
	{
		if(noise[i])
		{
			if(tempNoiseSize == 0 && noiseSize != 0)
			{
				interpolate(output, noiseSize, noiseCache, leftCache, rightCache);
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
				if(leftCache.size() >= mWindowSize)
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

void ViInterpolator::interpolate(ViSampleChunk &output, const int &noiseSize, QVector<qreal> &noiseCache, QVector<qreal> &leftCache, QVector<qreal> &rightCache)
{
	ViSampleChunk outputData(noiseSize + leftCache.size());
	for(int i = 0; i < leftCache.size(); ++i) outputData[i] = leftCache[i];
	if(noiseSize == 0)
	{
		output.append(outputData);
		return;
	}
	for(int i = 0; i < noiseCache.size(); ++i) outputData[i + leftCache.size()] = noiseCache[i];

	qreal *leftData = leftCache.data();
	qreal *rightData = rightCache.data();

	int leftSize = leftCache.size();
	int rightSize = rightCache.size();
	if(mHalfWindowSize > 0)
	{
		leftSize = qMin(leftSize, mHalfWindowSize);
		rightSize = qMin(rightSize, mHalfWindowSize);
		leftData += leftSize;
	}

	//cout<<"++:"<<leftSize<<"\t"<<noiseSize<<"\t"<<rightSize<<endl;

	interpolate(leftData, leftSize, rightData, rightSize, outputData.data() + leftCache.size(), noiseSize);
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
