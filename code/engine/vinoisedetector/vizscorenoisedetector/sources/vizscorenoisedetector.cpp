#include <vizscorenoisedetector.h>
#include <qmath.h>

#define WINDOW_SIZE 128

ViZscoreNoiseDetector::ViZscoreNoiseDetector()
	: ViNoiseDetector()
{
	mHalfWindow = WINDOW_SIZE / 2;
	setOffset(mHalfWindow);
}

void ViZscoreNoiseDetector::calculateNoise(QQueue<qreal> &samples)
{
	static int i;
	static qreal mean, standardDeviation, value;

	while(samples.size() >= WINDOW_SIZE + 1)
	{
		// Calculate the mean
		mean = 0;
		for(i = 0; i < mHalfWindow; ++i)
		{
			mean += samples[i];
		}
		for(i = mHalfWindow + 1; i < WINDOW_SIZE; ++i)
		{
			mean += samples[i];
		}
		mean /= WINDOW_SIZE;

		// Calculate the standard diviation
		standardDeviation = 0;
		for(i = 0; i < mHalfWindow; ++i)
		{
			standardDeviation += qPow(samples[i] - mean, 2);
		}
		for(i = mHalfWindow + 1; i < WINDOW_SIZE; ++i)
		{
			standardDeviation += qPow(samples[i] - mean, 2);
		}
		standardDeviation = qSqrt(standardDeviation / WINDOW_SIZE);

		// Calculate the z-score
		value = qAbs((samples[mHalfWindow] - mean) / (1 + standardDeviation));
		setNoise(value);

		samples.removeFirst();
	}
}

ViZscoreNoiseDetector* ViZscoreNoiseDetector::clone()
{
	return new ViZscoreNoiseDetector(*this);
}

#ifdef __cplusplus
extern "C"
{
#endif

ViZscoreNoiseDetector* create()
{
	return new ViZscoreNoiseDetector();
}

#ifdef __cplusplus
}
#endif
