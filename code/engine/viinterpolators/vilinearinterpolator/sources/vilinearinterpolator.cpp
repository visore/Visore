#include <vilinearinterpolator.h>

ViLinearInterpolator::ViLinearInterpolator()
	: ViInterpolator(1, 1)
{
}

qreal ViLinearInterpolator::calculate()
{
	return (mLeftData->last() + mRightData->first()) / 2.0;
}

ViSampleChunk ViLinearInterpolator::calculate(int samples)
{
	setRatioSamples(samples);
	qreal currentRatio;
	ViSampleChunk result(samples);
	for(int i = 0; i < samples; ++i)
	{
		currentRatio = ratio(i);
		result[i] = (mLeftData->last() * (1 - currentRatio)) + (mRightData->first() * currentRatio);
	}
	return result;
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
