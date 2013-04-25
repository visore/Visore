#include <visumofsquarederrorfunction.h>
#include <qmath.h>

ViErrorFunction* ViSumOfSquaredErrorFunction::clone()
{
	return new ViSumOfSquaredErrorFunction(*this);
}

void ViSumOfSquaredErrorFunction::clearValues()
{
	mSum = 0;
}

qreal ViSumOfSquaredErrorFunction::calculate(const qreal &realValue, const qreal &targetValue)
{
	mSum += qPow(realValue - targetValue, 2);
	return mSum;
}

qreal ViSumOfSquaredErrorFunction::calculate(const ViDoubleList &realValues, const ViDoubleList &targetValues)
{
	for(int i = 0; i < realValues.size(); ++i)
	{
		mSum += qPow(realValues[i] - targetValues[i], 2);
	}
	return mSum;
}
