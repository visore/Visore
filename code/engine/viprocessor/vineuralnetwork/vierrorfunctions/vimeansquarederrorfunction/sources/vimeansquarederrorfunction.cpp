#include <vimeansquarederrorfunction.h>
#include <qmath.h>

ViErrorFunction* ViMeanSquaredErrorFunction::clone()
{
	return new ViMeanSquaredErrorFunction(*this);
}

qreal ViMeanSquaredErrorFunction::calculate(const ViRealMatrix &realValues, const ViRealMatrix &targetValues)
{
	qreal error = 0;
	for(int i = 0; i < realValues.size(); ++i)
	{
		for(int j = 0; j < realValues[i].size(); ++j)
		{
			error += qPow(realValues[i][j] - targetValues[i][j], 2);
		}
	}
	error /= realValues.size();
	return error;
}
