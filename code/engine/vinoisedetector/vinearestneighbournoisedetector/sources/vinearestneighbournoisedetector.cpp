#include <vinearestneighbournoisedetector.h>

#define DEFAULT_K 32

ViNearestNeighbourNoiseDetector::ViNearestNeighbourNoiseDetector()
	: ViNoiseDetector()
{
	setK(DEFAULT_K);
}

ViNearestNeighbourNoiseDetector::ViNearestNeighbourNoiseDetector(const ViNearestNeighbourNoiseDetector &other)
	: ViNoiseDetector(other)
{
	mK = other.mK;
	mHalfK = other.mHalfK;
	mTotal = other.mTotal;
}

void ViNearestNeighbourNoiseDetector::setK(const int &k)
{
	mK = k;
	mHalfK = mK / 2;
	mTotal = mK + 1;
	setOffset(mHalfK);
}

void ViNearestNeighbourNoiseDetector::setParameters(const qreal &param1)
{
	setK(param1);
}

void ViNearestNeighbourNoiseDetector::calculateNoise(QQueue<qreal> &samples)
{
	static int i;
	static qreal totalDifference;

	while(samples.size() > mTotal)
	{
		const qreal &currentValue = samples[mHalfK];
		totalDifference = 0;

		for(i = 0; i < mHalfK; ++i) totalDifference += qAbs(currentValue - samples[i]);
		for(i = mHalfK + 1; i < mK; ++i) totalDifference += qAbs(currentValue - samples[i]);

		// Devide by mK, since we want the mean of all differences
		// Devide 2, since the maximum distance between samples can be 2
		setNoise(totalDifference / (mK * 2));

		samples.removeFirst();
	}
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
