#include <vimadnoisedetector.h>
#include <qmath.h>

#define MAD 0.6745
#define MAD_THRESHOLD 3.5
#define WINDOW_SIZE 128

ViMadNoiseDetector::ViMadNoiseDetector()
	: ViNoiseDetector()
{
	mHalfWindow = WINDOW_SIZE / 2;
	setOffset(mHalfWindow);
}

void ViMadNoiseDetector::calculateNoise(QQueue<qreal> &samples)
{
	static int i;
	static qreal median, mad, value;

	while(samples.size() >= WINDOW_SIZE)
	{
		// Calculate the median
		QQueue<qreal> ordered = samples;
		qSort(ordered);
		median = ordered[mHalfWindow];

		// Calculate MAD
		QQueue<qreal> deviations;
		for(i = 0; i < WINDOW_SIZE; ++i)
		{
			deviations.enqueue(qAbs(samples[i] - median));
		}
		qSort(deviations);
		mad = deviations[mHalfWindow];

		// Calculate the z-score
		value = qAbs((MAD * (samples[mHalfWindow] - median)) / (1 + mad));
		setNoise(value /*/ MAD_THRESHOLD*/);
		//cout<<value<<"\t"<<(MAD * (samples[mHalfWindow] - median))<<"\t"<<value<<endl;

		samples.removeFirst();
	}
}

ViMadNoiseDetector* ViMadNoiseDetector::clone()
{
	return new ViMadNoiseDetector(*this);
}

#ifdef __cplusplus
extern "C"
{
#endif

ViMadNoiseDetector* create()
{
	return new ViMadNoiseDetector();
}

#ifdef __cplusplus
}
#endif
