#include <vifourierinterpolator.h>
#include <vimodelsolver.h>

#include<vilogger.h>

#define DEFAULT_DEGREE 3

ViFourierInterpolator::ViFourierInterpolator()
	: ViAutoDegreeInterpolator(DEFAULT_DEGREE)
{
}

ViFourierInterpolator::ViFourierInterpolator(const int &degree)
	: ViAutoDegreeInterpolator(degree)
{
}

bool ViFourierInterpolator::interpolateSamples(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize)
{
	// http://cant.ua.ac.be/sites/cant.ua.ac.be/files/courses/cscw/ratint/fourier.fausett.pdf

	static qreal value1, value2, scaledX, multiplier, a0;
	static int i, j, sampleCount, devider;

	sampleCount = leftSize + rightSize;
	devider = sampleCount + outputSize;
	multiplier = (2 * M_PI) / devider;

	QList<qreal> a, b;
	for(j = 1; j <= mDegree; ++j)
	{
		value1 = 0;
		value2 = 0;
		for(i = 0; i < leftSize; ++i)
		{
			scaledX = multiplier * i;
			value1 += leftSamples[i] * qCos(j * scaledX);
			value2 += leftSamples[i] * qSin(j * scaledX);
		}
		for(i = 0; i < rightSize; ++i)
		{
			scaledX = multiplier * (i - leftSize);
			value1 += rightSamples[i] * qCos(j * scaledX);
			value2 += rightSamples[i] * qSin(j * scaledX);
		}
		a.append((2 * value1) / sampleCount);
		b.append((2 * value2) / sampleCount);
	}
	//if(sampleCount % 2 == 0) a.last() /= 2;

	// cos(0) = 1, so we can ignore cos
	a0 = 0;
	for(i = 0; i < leftSize; ++i)
	{
		a0 += leftSamples[i];
	}
	for(i = 0; i < rightSize; ++i)
	{
		a0 += rightSamples[i];
	}
	//a0 = a0 / sampleCount;
	a0 = (2 * a0) / sampleCount;

	for(i = 0; i < outputSize; ++i)
	{
		value1 = (i + leftSize) * multiplier;
		value2 = a0 + (a[0] * qCos(value1)) + (b[0] * qSin(value1));
		for(j = 2; j <= mDegree; ++j)
		{
			value2 += (a[j - 1] * qCos(j * value1)) + (b[j - 1] * qSin(j * value1));
		}
		outputSamples[i] = value2;
	}
}
/*
bool ViFourierInterpolator::interpolateSamplesSine(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize)
{
	if(leftSize == 0 || rightSize == 0) return false;
	static qreal currentRatio, ratio;
	ratio = 1.0 / (outputSize + 1);
	for(int i = 0; i < outputSize; ++i)
	{
		currentRatio = (1 - qSin((ratio * (i + 1)) * M_PI)) / 2;
		outputSamples[i] = (leftSamples[leftSize - 1] * (1 - currentRatio)) + (rightSamples[0] * currentRatio);
	}
	return true;
}

bool ViFourierInterpolator::interpolateSamplesCosine(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize)
{
	if(leftSize == 0 || rightSize == 0) return false;
	static qreal currentRatio, ratio;
	ratio = 1.0 / (outputSize + 1);
	for(int i = 0; i < outputSize; ++i)
	{
		currentRatio = (1 - qCos((ratio * (i + 1)) * M_PI)) / 2;
		outputSamples[i] = (leftSamples[leftSize - 1] * (1 - currentRatio)) + (rightSamples[0] * currentRatio);
	}
	return true;
}
*/

ViFourierInterpolator* ViFourierInterpolator::clone()
{
	return new ViFourierInterpolator(*this);
}

#ifdef __cplusplus
extern "C"
{
#endif

ViFourierInterpolator* create()
{
	return new ViFourierInterpolator();
}

#ifdef __cplusplus
}
#endif
