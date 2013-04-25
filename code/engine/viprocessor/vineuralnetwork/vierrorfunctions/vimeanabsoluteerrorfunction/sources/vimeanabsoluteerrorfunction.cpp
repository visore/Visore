#include <vimeanabsoluteerrorfunction.h>
#include <qmath.h>

ViErrorFunction* ViMeanAbsoluteErrorFunction::clone()
{
	return new ViMeanAbsoluteErrorFunction(*this);
}

void ViMeanAbsoluteErrorFunction::clearValues()
{
	mSum = 0;
}

qreal ViMeanAbsoluteErrorFunction::calculate(const qreal &realValue, const qreal &targetValue)
{
	mSum += qAbs(realValue - targetValue);
	return mSum / count();
}

qreal ViMeanAbsoluteErrorFunction::calculate(const ViDoubleList &realValues, const ViDoubleList &targetValues)
{
	for(int i = 0; i < realValues.size(); ++i)
	{
		mSum += qAbs(realValues[i] - targetValues[i]);
	}
	return mSum / count();
}
