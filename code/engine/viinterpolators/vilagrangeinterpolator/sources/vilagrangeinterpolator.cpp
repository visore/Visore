#include <vilagrangeinterpolator.h>
#include <vilogger.h>

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

bool ViLagrangeInterpolator::interpolate(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize)
{
	static int i, j, k, offset, size;
	static qreal result, product, scaledI, scaling;

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

	for(i = 0; i < outputSize; ++i)
	{
		scaledI = i / scaling;
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
		outputSamples[i] = result;
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
