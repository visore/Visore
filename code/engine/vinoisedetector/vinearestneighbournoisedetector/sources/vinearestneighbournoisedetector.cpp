#include <vinearestneighbournoisedetector.h>

#define DEFAULT_K 32
#define DEFAULT_MODE ODIN

ViNearestNeighbourNoiseDetector::ViNearestNeighbourNoiseDetector()
	: ViNoiseDetector()
{
	setOutlierMode(DEFAULT_MODE);
	setK(DEFAULT_K);
}

void ViNearestNeighbourNoiseDetector::setOutlierMode(const OutlierMode &mode)
{
	if(mode == ODIN)
	{
		calculatePointer = &ViNearestNeighbourNoiseDetector::calculateOdin;
	}
	else if(mode == MDIST)
	{
		calculatePointer = &ViNearestNeighbourNoiseDetector::calculateMdist;
	}
	else if(mode == KDIST)
	{
		calculatePointer = &ViNearestNeighbourNoiseDetector::calculateKdist;
	}
}

void ViNearestNeighbourNoiseDetector::setK(const int &k)
{
	mK = k;
	mHalfK = mK / 2;
}

void ViNearestNeighbourNoiseDetector::calculateNoise(const ViSampleChunk &samples)
{
	(this->*calculatePointer)(samples);
}

void ViNearestNeighbourNoiseDetector::generateGraph(const ViSampleChunk &samples, QList<qreal> &vectors)
{
	vectors.clear();
	static int end, end2, i, j;
	static qreal totalDifference;

	//First few samples cannot be tested, since no left samples are available
	if(mCache.isEmpty())
	{
		for(i = 0; i < mHalfK; ++i)
		{
			vectors.append(0);
		}
	}

	for(i = 0; i < samples.size(); ++i)
	{
		mCache.enqueue(samples[i]);
	}

	end = mCache.size() - (mK / 2);
	for(i = mHalfK; i < end; ++i)
	{
		const qreal &currentValue = mCache[i];
		totalDifference = 0;

		// Half mK samples before the current sample
		for(j = i - mHalfK; j < i; ++j)
		{
			totalDifference += qAbs(currentValue - mCache[j]);
		}

		// Half mK samples after the current sample
		end2 = i + mHalfK;
		for(j = i + 1; j <= end2; ++j)
		{
			totalDifference += qAbs(currentValue - mCache[j]);
		}

		// Devide by mK, since we want the mean of all differences
		// Devide 2, since the maximum distance between samples can be 2
		vectors.append(totalDifference / (mK * 2));
	}

	// We don't need the first samples anymore
	while(mCache.size() > mK)
	{
		mCache.removeFirst();
	}
}

void ViNearestNeighbourNoiseDetector::calculateOdin(const ViSampleChunk &samples)
{
	static QList<qreal> vectors;
	static int i;
	generateGraph(samples, vectors);

	for(i = 0; i < vectors.size(); ++i)
	{

		setNoise(vectors[i]);
	}
	vectors.clear();
}

void ViNearestNeighbourNoiseDetector::calculateMdist(const ViSampleChunk &samples)
{
	static QList<qreal> vectors;
	static int i;
	generateGraph(samples, vectors);

	for(i = 0; i < vectors.size(); ++i)
	{
		// Devide by mK, since we want the mean of all differences
		// Devide 2, since the maximum distance between samples can be 2
		setNoise(vectors[i] / (mK * 2));
	}
	vectors.clear();
}

void ViNearestNeighbourNoiseDetector::calculateKdist(const ViSampleChunk &samples)
{
	static QList<qreal> vectors;
	static int i;
	static qreal maximum;

	maximum = 0;
	generateGraph(samples, vectors);

	for(i = 0; i < vectors.size(); ++i)
	{
		if(vectors[i] > maximum) maximum = vectors[i];
	}

	for(i = 0; i < vectors.size(); ++i)
	{
		// Devide by mK, since we want the mean of all differences
		// Devide 2, since the maximum distance between samples can be 2
		setNoise(vectors[i] / (mK * 2));
	}
	vectors.clear();
}

ViNearestNeighbourNoiseDetector* ViNearestNeighbourNoiseDetector::clone()
{
	return new ViNearestNeighbourNoiseDetector(*this);
}

#ifdef __cplusplus
extern "C"
{
#endif

ViNearestNeighbourNoiseDetector* create()
{
	return new ViNearestNeighbourNoiseDetector();
}

#ifdef __cplusplus
}
#endif
