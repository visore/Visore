#include <visumofsquarederrorfunction.h>
#include <qmath.h>

ViErrorFunction* ViSumOfSquaredErrorFunction::clone()
{
	return new ViSumOfSquaredErrorFunction(*this);
}

qreal ViSumOfSquaredErrorFunction::calculate(const ViRealMatrix &realValues, const ViRealMatrix &targetValues)
{
	qreal error = 0;
	for(int i = 0; i < realValues.size(); ++i)
	{
		for(int j = 0; j < realValues[i].size(); ++j)
		{
			error += qPow(realValues[i][j] - targetValues[i][j], 2);
		}
	}
	return error;
}
