#include <vimeansquarederrorfunction.h>
#include <qmath.h>

ViErrorFunction* ViMeanSquaredErrorFunction::clone()
{
	return new ViMeanSquaredErrorFunction(*this);
}

void ViMeanSquaredErrorFunction::clearValues()
{
	mSum = 0;
}

qreal ViMeanSquaredErrorFunction::calculate(const qreal &realValue, const qreal &targetValue)
{
	mSum += qPow(realValue - targetValue, 2);
	return mSum / count();
}

qreal ViMeanSquaredErrorFunction::calculate(const ViDoubleList &realValues, const ViDoubleList &targetValues)
{
	for(int i = 0; i < realValues.size(); ++i)
	{
		mSum += qPow(realValues[i] - targetValues[i], 2);
	}
	return mSum / count();
}
