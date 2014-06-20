/*#include <vimadnoisedetector.h>
#include <vimath.h>
#include <qmath.h>

#define MAD 0.6745
#define MAD_THRESHOLD 3.5
#define WINDOW_SIZE 128
#define AMPLIFICATION 0.1

ViMadNoiseDetector::ViMadNoiseDetector()
	: ViNoiseDetector()
{
	mWindow = NULL;
	setWindowSize(WINDOW_SIZE);
	setAmplification(AMPLIFICATION);
	addParameterName("Window Size");
}

ViMadNoiseDetector::ViMadNoiseDetector(const ViMadNoiseDetector &other)
	: ViNoiseDetector(other)
{
	mWindow = NULL;
	setWindowSize(other.mWindowSize);
}

ViMadNoiseDetector::~ViMadNoiseDetector()
{
	if(mWindow != NULL)
	{
		delete [] mWindow;
		mWindow = NULL;
	}
}

void ViMadNoiseDetector::setWindowSize(const int &size)
{
	mWindowSize = size;
	mHalfWindowSize = qFloor(size / 2.0);
	if(mWindow != NULL) delete [] mWindow;
	mWindow = new qreal[mWindowSize];
	setOffset(mHalfWindowSize);
}

void ViMadNoiseDetector::setParameters(const qreal &param1)
{
	setWindowSize(param1);
}

void ViMadNoiseDetector::calculateNoise(QQueue<qreal> &samples)
{
	static int i;
	static qreal median, mad;

	while(samples.size() >= mWindowSize)
	{
		// Calculate the median
		for(i = 0; i < mWindowSize; ++i) mWindow[i] = samples[i];
		median = ViMath<qreal>::median(mWindow, mWindowSize, mHalfWindowSize);

		// Calculate MAD
		for(i = 0; i < mWindowSize; ++i) mWindow[i] = qAbs(samples[i] - median);
		mad = ViMath<qreal>::median(mWindow, mWindowSize, mHalfWindowSize);

		// Calculate the z-score
		setNoise((MAD * abs(samples[mHalfWindowSize] - median)) / mad); // Original MAD doesn't take the absolute value here

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
*/
