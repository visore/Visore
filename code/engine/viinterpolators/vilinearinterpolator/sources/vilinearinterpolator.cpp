#include <vilinearinterpolator.h>

#define DECAY 0.9

bool ViLinearInterpolator::interpolateSamples(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize)
{
	if(leftSize == 0)
	{
		if(rightSize == 0) return false;

		int i = outputSize - 1;
		qreal delta, value = rightSamples[0];
		if(rightSize < 2) delta = 0;
		else delta = rightSamples[0] - rightSamples[1];
		while(i >= 0)
		{
			value += delta;
			outputSamples[i] = value;
			value *= DECAY;
			delta *= DECAY;
			--i;
		}
	}
	else if(rightSize == 0)
	{
		int i = 0;
		qreal delta, value = leftSamples[leftSize - 1];
		if(leftSize < 2) delta = 0;
		else delta = leftSamples[leftSize - 1] - leftSamples[leftSize - 2];
		while(i < outputSize)
		{
			value += delta;
			outputSamples[i] = value;
			value *= DECAY;
			delta *= DECAY;
			++i;
		}
	}
	else
	{
		int i = 0;
		qreal value = leftSamples[leftSize - 1];
		qreal delta = (rightSamples[0] - leftSamples[leftSize - 1]) / (outputSize + 1);
		while(i < outputSize)
		{
			value += delta;
			outputSamples[i] = value;
			++i;
		}
	}
	return true;
}

ViLinearInterpolator* ViLinearInterpolator::clone()
{
	return new ViLinearInterpolator(*this);
}

#ifdef __cplusplus
extern "C"
{
#endif

ViLinearInterpolator* create()
{
	return new ViLinearInterpolator();
}

#ifdef __cplusplus
}
#endif
