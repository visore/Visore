#include <vimahalanobisnoisedetector.h>
#include <vivector.h>

#define WINDOW_SIZE 16
#define AMPLIFIER 10000

ViMahalanobisNoiseDetector::ViMahalanobisNoiseDetector()
	: ViNoiseDetector()
{
	setOffset(WINDOW_SIZE / 2);
}

void ViMahalanobisNoiseDetector::calculateNoise(QQueue<qreal> &samples)
{
	static int i;
	static qreal mean;

	while(samples.size() >= WINDOW_SIZE)
	{
		// Calculate the mean
		mean = 0;
		for(i = 0; i < WINDOW_SIZE; ++i)
		{
			mean += samples[i];
		}
		mean /= WINDOW_SIZE;

		// Create variances
		ViVector vector(WINDOW_SIZE);
		for(i = 0; i < WINDOW_SIZE; ++i)
		{
			vector[i] = samples[i] - mean;
		}

		qreal covariance = (vector * vector).sum() / (WINDOW_SIZE - 1);

		// Calculate the distance
		qreal distance = vector[(WINDOW_SIZE / 2) + 1];
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
