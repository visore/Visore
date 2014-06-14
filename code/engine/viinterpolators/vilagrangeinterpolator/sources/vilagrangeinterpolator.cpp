#include <vilagrangeinterpolator.h>
#include <vilogger.h>
#include <vieigen.h>

ViLagrangeInterpolator::ViLagrangeInterpolator()
	: ViInterpolator()
{
	addParameterName("Window Size");
}

ViLagrangeInterpolator::ViLagrangeInterpolator(const ViLagrangeInterpolator &other)
	: ViInterpolator(other)
{
}

ViLagrangeInterpolator::~ViLagrangeInterpolator()
{
}

void ViLagrangeInterpolator::setParameter(const int &number, const qreal &value)
{
	if(number == 0) setWindowSize(value);
	else
	{
		LOG("Invalid parameter for this interpolator.", QtCriticalMsg);
		exit(-1);
	}
}

bool ViLagrangeInterpolator::validParameters()
{
	return validParameters(mWindowSize);
}

bool ViLagrangeInterpolator::validParameters(const int &windowSize)
{
	return windowSize > 1;
}

bool ViLagrangeInterpolator::validParameters(const int &leftSize, const int &rightSize)
{
	return validParameters(leftSize + rightSize);
}

bool ViLagrangeInterpolator::interpolate(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error)
{
	// Doesn't matter if you use double or mpreal, same result

	static int i, offset, size;
	static qreal scaling;

	size = leftSize + rightSize;
	scaling = size + outputSize - 1;

	qreal x[size];
	qreal y[size];

	for(i = 0; i < leftSize; ++i)
	{
		x[i] = i / scaling;
		y[i] = leftSamples[i];
	}
	for(i = 0; i < rightSize; ++i)
	{
		offset = i + leftSize;
		x[offset] = (offset + outputSize) / scaling;
		y[offset] = rightSamples[i];
	}

	calculate(x, y, size, outputSamples, outputSize, leftSize, scaling);

	if(error != NULL)
	{
		qreal leftModel[leftSize];
		calculate(x, y, size, leftModel, leftSize, 0, scaling);
		error->add(leftModel, leftSamples, leftSize);

		qreal rightModel[rightSize];
		calculate(x, y, size, rightModel, rightSize, leftSize + outputSize, scaling);
		error->add(rightModel, rightSamples, rightSize);
	}

	return true;
}

void ViLagrangeInterpolator::calculate(const qreal *x, const qreal *y, const int &size, qreal *output, const int &outputSize, const int &startX, const qreal &scaling)
{
	static int i, j, k;
	static qreal result, product, scaledI;

	for(i = 0; i < outputSize; ++i)
	{
		scaledI = (i + startX) / scaling;
		result = 0;
		for(j = 0; j < size; ++j)
		{
			product = 1;
			for(k = 0; k < size; ++k)
			{
				if(j != k) product *= (scaledI - x[k]) / (x[j] - x[k]);
			}
			result += y[j] * product;
		}
		output[i] = result;
	}
}

ViLagrangeInterpolator* ViLagrangeInterpolator::clone()
{
	return new ViLagrangeInterpolator(*this);
}

#ifdef __cplusplus
extern "C"
{
#endif

ViLagrangeInterpolator* create()
{
	return new ViLagrangeInterpolator();
}

#ifdef __cplusplus
}
#endif
