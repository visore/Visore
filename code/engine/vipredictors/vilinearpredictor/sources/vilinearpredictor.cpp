#include <vilinearpredictor.h>

ViLinearPredictor* ViLinearPredictor::clone()
{
	return new ViLinearPredictor(*this);
}

qreal ViLinearPredictor::predictValue(const qreal *samples, const int &size)
{
	mSolver.setOrder(size - 1);
	ViVector vector(size, (qreal*) samples, true);
	if(mSolver.estimate(vector))
	{
		ViVector coefficients = mSolver.coefficients();
		qreal prediction = coefficients[0];
		for(int i = 1; i < coefficients.size(); ++i)
		{
			prediction += coefficients[i] * qPow(size, i);
		}
		return prediction;
	}
	return 0;
}

QList<qreal> ViLinearPredictor::predictValues(const qreal *samples, const int &size, const int &length)
{
	mSolver.setOrder(size - 1);
	QList<qreal> result;
	ViVector vector(size, (qreal*) samples, true);
	if(mSolver.estimate(vector))
	{
		ViVector coefficients = mSolver.coefficients();
		int l, i, x;
		qreal prediction;
		for(l = 0; l < length; ++l)
		{
			x = size + l;
			prediction = coefficients[0];
			for(i = 1; i < coefficients.size(); ++i)
			{
				prediction += coefficients[i] * qPow(x, i);
			}
			result.append(prediction);
		}
	}
	return result;
}

#ifdef __cplusplus
extern "C" {
#endif

ViLinearPredictor* create()
{
	return new ViLinearPredictor();
}

#ifdef __cplusplus
}
#endif
