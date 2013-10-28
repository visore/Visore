#include <vimahalanobisnoisedetector.h>
#include <vimatrix.h>

ViMahalanobisNoiseDetector::ViMahalanobisNoiseDetector()
	: ViNoiseDetector()
{
}

void ViMahalanobisNoiseDetector::calculateNoise(const ViSampleChunk &samples)
{
	int i, size = samples.size();

	// Calculate the mean
	qreal meanX = 0, meanY = 0;
	for(i = 0; i < size; ++i)
	{
		meanX += i;
		meanY += samples[i];
	}
	meanX /= size;
	meanY /= size;

	// Create variances
	ViVector vectorX(size);
	ViVector vectorY(size);
	for(i = 0; i < size; ++i)
	{
		vectorX[i] = i - meanX;
		vectorY[i] = samples[i] - meanY;
	}

	// Create the covariance matrix
	int statsSize = size - 1;
	qreal covariance = ((vectorY * vectorY).sum() / statsSize);
	/*ViMatrix covariance(2, 2);
	covariance[0][0] = (vectorX * vectorX).sum() / statsSize;
	covariance[0][1] = (vectorX * vectorY).sum() / statsSize;
	covariance[1][0] = covariance[0][1];
	covariance[1][1] = (vectorY * vectorY).sum() / statsSize;*/

	// Calculate the distance
	qreal distance;
	ViVector vector(2);
	for(i = 0; i < size; ++i)
	{
		vector[0] = vectorX[i];
		vector[1] = vectorY[i];
		//distance = (vector.transpose() * covariance.invert() * vector)[0];
		distance = (vectorY[i] * covariance * vectorY[i]);
		//noise.set(i, qAbs(distance));
		setNoise(qAbs(distance));
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
