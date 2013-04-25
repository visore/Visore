#include <virootmeansquarederrorfunction.h>
#include <qmath.h>

ViErrorFunction* ViRootMeanSquaredErrorFunction::clone()
{
	return new ViRootMeanSquaredErrorFunction(*this);
}

void ViRootMeanSquaredErrorFunction::clearValues()
{
	mSum = 0;
}

qreal ViRootMeanSquaredErrorFunction::calculate(const qreal &realValue, const qreal &targetValue)
{
	mSum += qPow(realValue - targetValue, 2);
	return qSqrt(mSum / count());
}

qreal ViRootMeanSquaredErrorFunction::calculate(const ViDoubleList &realValues, const ViDoubleList &targetValues)
{
	for(int i = 0; i < realValues.size(); ++i)
	{
		mSum += qPow(realValues[i] - targetValues[i], 2);
	}
	return qSqrt(mSum / count());
}
