#include <vinearestneighbourinterpolator.h>
#include <vimath.h>

#define DEFAULT_K 1

ViNearestNeighbourInterpolator::ViNearestNeighbourInterpolator(const Mode &mode)
	: ViInterpolator()
{
	setK(DEFAULT_K);
	setMode(mode);
	addParameterName("K");
}

ViNearestNeighbourInterpolator::ViNearestNeighbourInterpolator(const ViNearestNeighbourInterpolator &other)
	: ViInterpolator(other)
{
	mMode = other.mMode;
	mK = other.mK;
}

ViNearestNeighbourInterpolator::~ViNearestNeighbourInterpolator()
{
}


QString ViNearestNeighbourInterpolator::name(QString replace, bool spaced)
{
	QString mode;

	if(mMode == Mean) mode = "Mean";
	else if(mMode == Median) mode = "Median";

	QString name = ViInterpolator::name(replace, spaced) + " (" + mode + ")";
	if(spaced) return name;
	else return name.replace(" ", "");
}

void ViNearestNeighbourInterpolator::setMode(const Mode &mode)
{
	mMode = mode;
	if(mMode == Mean) interpolatePointer = &ViNearestNeighbourInterpolator::interpolateMean;
	else if(mMode == Median) interpolatePointer = &ViNearestNeighbourInterpolator::interpolateMedian;
}

void ViNearestNeighbourInterpolator::setK(const int &k)
{
	mK = k;
}

void ViNearestNeighbourInterpolator::setParameter(const int &number, const qreal &value)
{
	if(number == 0) setK(value);
	else
	{
		LOG("Invalid parameter for this interpolator.", QtCriticalMsg);
		exit(-1);
	}
}

bool ViNearestNeighbourInterpolator::validParameters()
{
	return mK > 0;
}

bool ViNearestNeighbourInterpolator::interpolate(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error, const int &channel)
{
	return (this->*interpolatePointer)(leftSamples, leftSize, rightSamples, rightSize, outputSamples, outputSize);
}

bool ViNearestNeighbourInterpolator::interpolateMean(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize)
{
	static int i, useLeft, useRight, leftStart;

	for(i = 0; i < outputSize; ++i)
	{
		nearest(leftSize, rightSize, outputSize, leftSize + i, mK, useLeft, useRight);
		leftStart = leftSize - useLeft;
		outputSamples[i] = ViMath<qreal>::mean(leftSamples + leftStart, leftSize - leftStart, rightSamples, useRight);
	}

	return true;
}

bool ViNearestNeighbourInterpolator::interpolateMedian(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize)
{
	static int i, j, useLeft, useRight, leftStart, size;

	for(i = 0; i < outputSize; ++i)
	{
		nearest(leftSize, rightSize, outputSize, leftSize + i, mK, useLeft, useRight);
		leftStart = leftSize - useLeft;

		size = useLeft + useRight;
		qreal temp[size];
		for(j = 0; j < useLeft; ++j) temp[j] = leftSamples[leftStart + j];
		for(j = 0; j < useRight; ++j) temp[j + useLeft] = rightSamples[j];

		outputSamples[i] = ViMath<qreal>::median(temp, size);
	}

	return true;
}

inline void ViNearestNeighbourInterpolator::nearest(const int &leftSize, const int &rightSize, const int &outputSize, const int &x, const int &k, int &useLeft, int &useRight)
{
	static int usedK, leftEnd, rightStart;

	useLeft = 0;
	useRight = 0;

	usedK = 0;
	leftEnd = leftSize - 1;
	rightStart = leftSize + outputSize;

	while(usedK != k)
	{
		if(rightStart >= rightSize)
		{
			if(leftEnd < 0) break;
			else
			{
				++useLeft;
				--leftEnd;
			}
		}
		else if(leftEnd < 0)
		{
			++useRight;
			++rightStart;
		}
		else if(abs(x - rightStart) < abs(x - leftEnd))
		{
			++useRight;
			++rightStart;
		}
		else
		{
			++useLeft;
			--leftEnd;
		}
		++usedK;
	}
}

ViNearestNeighbourInterpolator* ViNearestNeighbourInterpolator::clone()
{
	return new ViNearestNeighbourInterpolator(*this);
}

#ifdef __cplusplus
extern "C"
{
#endif

ViNearestNeighbourInterpolator* create()
{
	return new ViNearestNeighbourInterpolator();
}

#ifdef __cplusplus
}
#endif

