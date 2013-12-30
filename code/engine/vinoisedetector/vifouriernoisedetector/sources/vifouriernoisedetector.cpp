#include <vifouriernoisedetector.h>

#define WINDOW_SIZE 64
#define SPECTRUM_USED 0.25 // The percentage of the last part of the spectrum to use

ViFourierNoiseDetector::ViFourierNoiseDetector()
	: ViNoiseDetector()
{
	mTransformer = new ViFourierTransformer();
	mTransformer->setSize(WINDOW_SIZE);
	setOffset(WINDOW_SIZE / 2);
}

ViFourierNoiseDetector::ViFourierNoiseDetector(const ViFourierNoiseDetector &other)
	: ViNoiseDetector(other)
{
	mTransformer = new ViFourierTransformer();
	mTransformer->setSize(WINDOW_SIZE);
	setOffset(WINDOW_SIZE / 2);
}

ViFourierNoiseDetector::~ViFourierNoiseDetector()
{
	delete mTransformer;
}

void ViFourierNoiseDetector::calculateNoise(QQueue<qreal> &samples)
{
	static int i;
	static qreal input[WINDOW_SIZE];
	static qreal output[WINDOW_SIZE];
	static qreal value;

	while(samples.size() >= WINDOW_SIZE)
	{
		for(i = 0; i < WINDOW_SIZE; ++i)
		{
			input[i] = samples[i];
		}
		mTransformer->forwardTransform(input, output);

		value = 0;
		for(i = WINDOW_SIZE * (1 - SPECTRUM_USED); i < WINDOW_SIZE; ++i)
		{
			value += qAbs(output[i-1] - output[i]);
		}
		value /= WINDOW_SIZE * SPECTRUM_USED;
		setNoise(value);
		samples.removeFirst();
	}
}

ViFourierNoiseDetector* ViFourierNoiseDetector::clone()
{
	return new ViFourierNoiseDetector(*this);
}

#ifdef __cplusplus
extern "C"
{
#endif

ViFourierNoiseDetector* create()
{
	return new ViFourierNoiseDetector();
}

#ifdef __cplusplus
}
#endif
