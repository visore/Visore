#include <vimadnoisedetector.h>
#include <vimath.h>
#include <qmath.h>

#define MAD 0.6745
#define WINDOW_SIZE 128

ViMadNoiseDetector::ViMadNoiseDetector()
	: ViNoiseDetector()
{
	mWindow = NULL;
	setScale(30);
	setWindowSize(WINDOW_SIZE);
	addParameter("Window Size");
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

bool ViMadNoiseDetector::validParameters()
{
	return parameter("Window Size") > 0;
}

void ViMadNoiseDetector::initialize()
{
	setWindowSize(parameter("Window Size"));
}

void ViMadNoiseDetector::detect(QVector<qreal> &samples, QVector<qreal> &noise)
{
	static int i;
	static qreal median, mad;

	while(samples.size() >= mWindowSize)
	{
		// Calculate the median
		for(i = 0; i < mWindowSize; ++i) mWindow[i] = samples[i];
		median = ViMath<qreal>::median(mWindow, mWindowSize, mHalfWindowSize);

		// Calculate MAD
		for(i = 0; i < mWindowSize; ++i) mWindow[i] = abs(samples[i] - median);
		mad = ViMath<qreal>::median(mWindow, mWindowSize, mHalfWindowSize);

		// Calculate the z-score
		noise.append(abs((MAD * (samples[mHalfWindowSize] - median)) / mad)); // Original MAD doesn't take the absolute value here

		samples.removeFirst();
	}
}
