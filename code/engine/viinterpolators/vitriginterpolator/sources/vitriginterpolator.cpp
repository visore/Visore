#include <vitriginterpolator.h>
#include <vimodelsolver.h>

#include<vilogger.h>

#define MAXIMUM_SAMPLES 6 // Maximum size of matrix. If too big, computation is very slow

ViTrigInterpolator::ViTrigInterpolator()
	: ViInterpolator(MAXIMUM_SAMPLES)
{
	setMode(Cosine);
}

ViTrigInterpolator::ViTrigInterpolator(const Mode &mode)
	: ViInterpolator(MAXIMUM_SAMPLES)
{
	setMode(mode);
}

void ViTrigInterpolator::setMode(const Mode &mode)
{
	mMode = mode;
	if(mMode == Sine) interpolateSamplesPointer = &ViTrigInterpolator::interpolateSamplesSine;
	else if(mMode == Cosine) interpolateSamplesPointer = &ViTrigInterpolator::interpolateSamplesCosine;
	else interpolateSamplesPointer = &ViTrigInterpolator::interpolateSamplesFourier;
}

ViTrigInterpolator::Mode ViTrigInterpolator::mode() const
{
	return mMode;
}

bool ViTrigInterpolator::interpolateSamples(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize)
{
	return (this->*interpolateSamplesPointer)(leftSamples, leftSize, rightSamples, rightSize, outputSamples, outputSize);
}

bool ViTrigInterpolator::interpolateSamplesSine(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize)
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

bool ViTrigInterpolator::interpolateSamplesCosine(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize)
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

bool ViTrigInterpolator::interpolateSamplesFourier(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize)
{
	// http://cant.ua.ac.be/sites/cant.ua.ac.be/files/courses/cscw/ratint/fourier.fausett.pdf

	static qreal value1, value2, scaledX, multiplier, a0;
	static int i, j, sampleCount, devider, degree;

	sampleCount = leftSize + rightSize;
	devider = sampleCount + outputSize;
	multiplier = (2 * M_PI) / devider;
	degree = qFloor(sampleCount / 2.0);

	QList<qreal> a, b;
	for(j = 1; j <= degree; ++j)
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
	if(sampleCount % 2 == 0) a[degree - 1] /= 2;

	a0 = 0;
	for(i = 0; i < leftSize; ++i)
	{
		a0 += leftSamples[i];
	}
	for(i = 0; i < rightSize; ++i)
	{
		a0 += rightSamples[i];
	}
	a0 = (2 * a0) / sampleCount;

	for(i = 0; i < outputSize; ++i)
	{
		value1 = (i + leftSize) * multiplier;
		value2 = a0 + (a[0] * qCos(value1)) + (b[0] * qSin(value1));
		for(j = 2; j <= degree; ++j)
		{
			value2 += (a[j - 1] * qCos(j * value1)) + (b[j - 1] * qSin(j * value1));
		}
		outputSamples[i] = value2;
	}

}

ViTrigInterpolator* ViTrigInterpolator::clone()
{
	return new ViTrigInterpolator(*this);
}

#ifdef __cplusplus
extern "C"
{
#endif

ViTrigInterpolator* create()
{
	return new ViTrigInterpolator();
}

#ifdef __cplusplus
}
#endif
