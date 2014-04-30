#include <vizscorenoisedetector.h>
#include <qmath.h>

#define DEFAULT_WINDOW_SIZE 128
#define DEFAULT_AMPLIFIER 0.05

ViZscoreNoiseDetector::ViZscoreNoiseDetector()
	: ViNoiseDetector()
{
	setWindowSize(DEFAULT_WINDOW_SIZE);
	setAmplification(DEFAULT_AMPLIFIER);
}

void ViZscoreNoiseDetector::setWindowSize(const int &size)
{
	mWindowSize = size;
	mHalfWindow = size / 2;
	setOffset(mHalfWindow);
}

void ViZscoreNoiseDetector::setParameters(const qreal &param1)
{
	setWindowSize(param1);
}

void ViZscoreNoiseDetector::calculateNoise(QQueue<qreal> &samples)
{
	static int i, end;
	static qreal mean, standardDeviation;

	end = mWindowSize + 1;
	while(samples.size() >= end)
	{
		// Calculate the mean
		mean = 0;
		for(i = 0; i < mHalfWindow; ++i)
		{
			mean += samples[i];
		}
		for(i = mHalfWindow + 1; i < mWindowSize; ++i)
		{
			mean += samples[i];
		}
		mean /= mWindowSize;

		// Calculate the standard diviation
		standardDeviation = 0;
		for(i = 0; i < mHalfWindow; ++i)
		{
			standardDeviation += qPow(samples[i] - mean, 2);
		}
		for(i = mHalfWindow + 1; i < mWindowSize; ++i)
		{
			standardDeviation += qPow(samples[i] - mean, 2);
		}
		standardDeviation = qSqrt(standardDeviation / (mWindowSize + 1)); // Corrected STD

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
