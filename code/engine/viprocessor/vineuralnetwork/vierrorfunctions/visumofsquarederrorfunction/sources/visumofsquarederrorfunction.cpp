#include <visumofsquarederrorfunction.h>
#include <qmath.h>

ViErrorFunction* ViSumOfSquaredErrorFunction::clone()
{
	return new ViSumOfSquaredErrorFunction(*this);
}

void ViSumOfSquaredErrorFunction::clear()
{
	ViErrorFunction::clear();
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

#ifdef __cplusplus
extern "C"
{
#endif

ViErrorFunction* create()
{
	return new ViSumOfSquaredErrorFunction();
}

#ifdef __cplusplus
}
#endif
