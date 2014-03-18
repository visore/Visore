#include <vimahalanobisnoisedetector.h>
#include <vivector.h>

#define DEFAULT_WINDOW_SIZE 64
#define AMPLIFIER 10

ViMahalanobisNoiseDetector::ViMahalanobisNoiseDetector()
	: ViNoiseDetector()
{
	setWindowSize(DEFAULT_WINDOW_SIZE);
}

void ViMahalanobisNoiseDetector::setWindowSize(int size)
{
	mWindowSize = size;
	setOffset(mWindowSize / 2);
}

void ViMahalanobisNoiseDetector::setParameters(qreal param1)
{
	setWindowSize(param1);
}

void ViMahalanobisNoiseDetector::calculateNoise(QQueue<qreal> &samples)
{
	static int i;
	static qreal mean;

	while(samples.size() >= mWindowSize)
	{
		// Calculate the mean
		mean = 0;
		for(i = 0; i < mWindowSize; ++i)
		{
			mean += samples[i];
		}
		mean /= mWindowSize;

		// Create variances
		ViVector vector(mWindowSize);
		for(i = 0; i < mWindowSize; ++i)
		{
			vector[i] = samples[i] - mean;
		}

		qreal covariance = (vector * vector).sum() / (mWindowSize - 1);

		// Calculate the distance
		qreal distance = vector[(mWindowSize / 2) + 1];
		distance = distance * covariance * distance;
		setNoise(qAbs(distance) * AMPLIFIER);

		samples.removeFirst();
	}
}

ViMahalanobisNoiseDetector* ViMahalanobisNoiseDetector::clone()
{
	return new ViMahalanobisNoiseDetector(*this);
}

#ifdef __cplusplus
extern "C"
{
#endif

ViMahalanobisNoiseDetector* create()
{
	return new ViMahalanobisNoiseDetector();
}

#ifdef __cplusplus
}
#endif
