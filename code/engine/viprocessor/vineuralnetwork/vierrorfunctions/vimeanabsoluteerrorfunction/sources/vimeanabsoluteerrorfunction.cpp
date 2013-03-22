#include <vimeanabsoluteerrorfunction.h>
#include <qmath.h>

ViErrorFunction* ViMeanAbsoluteErrorFunction::clone()
{
	return new ViMeanAbsoluteErrorFunction(*this);
}

qreal ViMeanAbsoluteErrorFunction::calculate(const ViRealMatrix &realValues, const ViRealMatrix &targetValues)
{
	qreal error = 0;
	for(int i = 0; i < realValues.size(); ++i)
	{
		for(int j = 0; j < realValues[i].size(); ++j)
		{
			error += qAbs(realValues[i][j] - targetValues[i][j]);
		}
	}
	error /= realValues.size();
	return error;
}
