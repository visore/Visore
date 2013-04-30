#include <vilinearinterpolator.h>

qreal ViLinearInterpolator::calculate()
{
	 return (mLeftData->last() + mRightData->first()) / 2.0;
}

ViSampleChunk ViLinearInterpolator::calculate(int samples)
{
	qreal ratio = 1.0 / (samples + 1);
	qreal currentRatio;
	ViSampleChunk result(samples);
	for(int i = 0; i < samples; ++i)
	{
		currentRatio = ratio * (i + 1);
		result[i] = (mLeftData->last() * (1 - currentRatio)) + (mLeftData->last() * currentRatio);
	}
	return result;
}

ViInterpolator* ViLinearInterpolator::clone()
{
	return new ViLinearInterpolator(*this);
}
