#include <vipiecewiseconstantinterpolator.h>

bool ViPiecewiseConstantInterpolator::interpolateSamples(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize)
{
	if(leftSize == 0 && rightSize == 0) return false;

	static int i, half;
	static qreal left, right;

	half = outputSize / 2;
	left = leftSamples[leftSize - 1];
	right = rightSamples[0];

	for(i = 0; i < half; ++i)
	{
		outputSamples[i] = left;
	}
	for(i = half; i < outputSize; ++i)
	{
		outputSamples[i] = right;
	}

	return true;
}

ViPiecewiseConstantInterpolator* ViPiecewiseConstantInterpolator::clone()
{
	return new ViPiecewiseConstantInterpolator(*this);
}

#ifdef __cplusplus
extern "C"
{
#endif

ViPiecewiseConstantInterpolator* create()
{
	return new ViPiecewiseConstantInterpolator();
}

#ifdef __cplusplus
}
#endif
