#include <vicosineinterpolator.h>

ViCosineInterpolator::ViCosineInterpolator()
	: ViInterpolator()
{
}

bool ViCosineInterpolator::interpolateSamples(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize)
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

ViCosineInterpolator* ViCosineInterpolator::clone()
{
	return new ViCosineInterpolator(*this);
}

#ifdef __cplusplus
extern "C"
{
#endif

ViCosineInterpolator* create()
{
	return new ViCosineInterpolator();
}

#ifdef __cplusplus
}
#endif
