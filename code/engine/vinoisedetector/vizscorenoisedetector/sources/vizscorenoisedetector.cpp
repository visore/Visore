#include <vizscorenoisedetector.h>
#include <qmath.h>

#define DEFAULT_WINDOW_SIZE 128
#define DEFAULT_AMPLIFIER 0.05

ViZscoreNoiseDetector::ViZscoreNoiseDetector()
	: ViNoiseDetector()
{
	setWindowSize(DEFAULT_WINDOW_SIZE);
	setAmplification(DEFAULT_AMPLIFIER);
	addParameterName("Window Size");
}

void ViZscoreNoiseDetector::setWindowSize(const int &size)
{
	mWindowSize = size;
	mCorrectedWindowSize = mWindowSize + 1;
	mHalfWindow = qFloor(size / 2.0);
	setOffset(mHalfWindow);
}

void ViZscoreNoiseDetector::setParameters(const qreal &param1)
{
	setWindowSize(param1);
}

void ViZscoreNoiseDetector::calculateNoise(QQueue<qreal> &samples)
{
	static int i;
	static qreal mean, standardDeviation;

	while(samples.size() >= mWindowSize)
	{
		// Calculate the unbiased sample mean
		mean = 0;
		for(i = 0; i < mWindowSize; ++i) mean += samples[i];
		mean /= mCorrectedWindowSize;

		// Calculate the corrected sample standard diviation
		standardDeviation = 0;
		for(i = 0; i < mWindowSize; ++i) standardDeviation += qPow(samples[i] - mean, 2);
		standardDeviation = qSqrt(standardDeviation / mCorrectedWindowSize);

		// Calculate the z-score
		setNoise(qAbs((samples[mHalfWindow] - mean) / standardDeviation));

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
