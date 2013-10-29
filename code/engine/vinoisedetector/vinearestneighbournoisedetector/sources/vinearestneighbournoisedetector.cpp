#include <vinearestneighbournoisedetector.h>

#define DEFAULT_K 32

ViNearestNeighbourNoiseDetector::ViNearestNeighbourNoiseDetector()
	: ViNoiseDetector()
{
	setK(DEFAULT_K);
	setOffset(mHalfK);
}

void ViNearestNeighbourNoiseDetector::setK(const int &k)
{
	mK = k;
	mHalfK = mK / 2;
}

void ViNearestNeighbourNoiseDetector::calculateNoise(QQueue<qreal> &samples)
{
	static int end, end2, i, j;
	static qreal totalDifference;

	end = samples.size() - (mK / 2);
	for(i = mHalfK; i < end; ++i)
	{
		const qreal &currentValue = samples[i];
		totalDifference = 0;

		// Half mK samples before the current sample
		for(j = i - mHalfK; j < i; ++j)
		{
			totalDifference += qAbs(currentValue - samples[j]);
		}

		// Half mK samples after the current sample
		end2 = i + mHalfK;
		for(j = i + 1; j <= end2; ++j)
		{
			totalDifference += qAbs(currentValue - samples[j]);
		}

		// Devide by mK, since we want the mean of all differences
		// Devide 2, since the maximum distance between samples can be 2
		setNoise(totalDifference / (mK * 2));
	}

	// We don't need the first samples anymore
	while(samples.size() > mK)
	{
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
