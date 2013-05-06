#include <vicubicinterpolator.h>

qreal ViCubicInterpolator::calculate()
{
	static qreal theRatio = 0.5;
	static qreal currentRatio = qPow(theRatio, 2);
	qreal left1 = mLeftData->last(1);
	qreal left2 = mLeftData->last();
	qreal right1 = mRightData->first();
	qreal right2 = mRightData->first(1);
	qreal a = right2 - right1 - left1 + left2;
	return (a * theRatio * currentRatio) + ((left1 - left2 - a) * currentRatio) + ((right1 - left1) * theRatio) + left2;
}

ViSampleChunk ViCubicInterpolator::calculate(int samples)
{
	setRatioSamples(samples);
	ViSampleChunk result(samples);
	qreal theRatio, currentRatio, left1, left2, right1, right2, a;
	for(int i = 0; i < samples; ++i)
	{
		theRatio = ratio(i);
		currentRatio = qPow(theRatio, 2);
		left1 = mLeftData->last(1);
		left2 = mLeftData->last();
		right1 = mRightData->first();
		right2 = mRightData->first(1);
		a = right2 - right1 - left1 + left2;
		result[i] = (a * theRatio * currentRatio) + ((left1 - left2 - a) * currentRatio) + ((right1 - left1) * theRatio) + left2;
	}
	return result;
}

ViCubicInterpolator* ViCubicInterpolator::clone()
{
	return new ViCubicInterpolator(*this);
}

#ifdef __cplusplus
extern "C"
{
#endif

ViCubicInterpolator* create()
{
	return new ViCubicInterpolator();
}

#ifdef __cplusplus
}
#endif
