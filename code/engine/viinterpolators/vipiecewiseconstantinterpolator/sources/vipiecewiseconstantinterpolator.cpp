#include <vipiecewiseconstantinterpolator.h>

bool ViPiecewiseConstantInterpolator::interpolateSamples(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize)
{
	if(leftSize == 0 && rightSize == 0)
	{
		return false;
	}
	int half = outputSize / 2;
	qreal left = leftSamples[leftSize - 1];
	qreal right = rightSamples[0];
	for(int i = 0; i < outputSize; ++i)
	{
		if(i < half) outputSamples[i] = left;
		else outputSamples[i] = right;
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
