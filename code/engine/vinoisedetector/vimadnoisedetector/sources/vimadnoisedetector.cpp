#include <vimadnoisedetector.h>
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
		median = quickMedian(mWindow, mWindowSize, mHalfWindowSize);

		// Calculate MAD
		for(i = 0; i < mWindowSize; ++i) mWindow[i] = qAbs(samples[i] - median);
		mad = quickMedian(mWindow, mWindowSize, mHalfWindowSize);

		// Calculate the z-score
		setNoise((MAD * qAbs(samples[mHalfWindowSize] - median)) / mad); // Original MAD doesn't take the absolute value here

		samples.removeFirst();
	}
}

#define SWAP(a,b) { temp = a; a = b; b = temp; } // Curly-brackets needed, so that SWAP can be used on a signle-line if-statement
qreal ViMadNoiseDetector::quickMedian(qreal *samples, const int &size, const int &position)
{
	// The quickselect algorithm
	// http://www.stat.cmu.edu/~ryantibs/median/

	static quint64 i, ir, j, l, m, mid;
	static qreal a, temp; // temp is used by SWAP

	l = 0;
	ir = size - 1;
	while(true)
	{
		m = l + 1;
		if(ir <= m)
		{
			if(ir == m && samples[ir] < samples[l]) SWAP(samples[l], samples[ir]);
			return samples[position];
		}
		else
		{
			mid = (l + ir) >> 1;
			SWAP(samples[mid], samples[m]);
			if(samples[l] > samples[ir]) SWAP(samples[l], samples[ir]);
			if(samples[m] > samples[ir]) SWAP(samples[m], samples[ir]);
			if(samples[l] > samples[m]) SWAP(samples[l], samples[m]);
			i = l + 1;
			j = ir;
			a = samples[m];
			while(true)
			{
				do ++i; while(samples[i] < a);
				do --j; while(samples[j] > a);
				if(j < i) break;
				SWAP(samples[i], samples[j]);
			}
			samples[m] = samples[j];
			samples[j] = a;
			if(j >= position) ir = j - 1;
			if(j <= position) l = i;
		}
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
