#include <virootmeansquarederrorfunction.h>
#include <qmath.h>

ViErrorFunction* ViRootMeanSquaredErrorFunction::clone()
{
	return new ViRootMeanSquaredErrorFunction(*this);
}

void ViRootMeanSquaredErrorFunction::clear()
{
	ViErrorFunction::clear();
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

#ifdef __cplusplus
extern "C"
{
#endif

ViErrorFunction* create()
{
	return new ViRootMeanSquaredErrorFunction();
}

#ifdef __cplusplus
}
#endif
