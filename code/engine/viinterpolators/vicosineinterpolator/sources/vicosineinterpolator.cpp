#include <vicosineinterpolator.h>

qreal ViCosineInterpolator::calculate()
{
	static qreal currentRatio = (1 - qCos(M_PI / 2)) / 2;
	return (mLeftData->last() * (1 - currentRatio)) + (mRightData->first() * currentRatio);
}

ViSampleChunk ViCosineInterpolator::calculate(int samples)
{
	setRatioSamples(samples);
	qreal currentRatio;
	ViSampleChunk result(samples);
	for(int i = 0; i < samples; ++i)
	{
		currentRatio = (1 - qCos(ratio(i) * M_PI)) / 2;
		result[i] = (mLeftData->last() * (1 - currentRatio)) + (mRightData->first() * currentRatio);
	}
	return result;
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
