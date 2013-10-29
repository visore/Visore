#include <vipredictionnoisedetector.h>
#include <visystemsolver.h>

ViPredictionNoiseDetector::ViPredictionNoiseDetector(const int &degree)
	: ViNoiseDetector()
{
	setDegree(degree);
	setOffset(mDegree);
}

void ViPredictionNoiseDetector::setDegree(const int &degree)
{
	mDegree = degree;
	int i, j, size = mDegree + 1;
	mMatrix = ViMatrix(size, mDegree + 1);
	for(i = 0; i < size; ++i)
	{
		mMatrix[i][0] = 1;
		for(j = 1; j <= mDegree; ++j)
		{
			mMatrix[i][j] = qPow(i, j);
		}
		mPowers.append(qPow(mDegree, mDegree - i));
	}
}

int ViPredictionNoiseDetector::degree()
{
	return mDegree;
}

void ViPredictionNoiseDetector::calculateNoise(QQueue<qreal> &samples)
{
	static int i, count, size;

	size = mDegree + 1;
	ViVector vector(size);
	static ViVector coefficients;

	while(samples.size() > size)
	{
		for(i = 0; i < size; ++i)
		{
			vector[i] = samples[i];
		}

		if(ViSystemSolver::solve(mMatrix, vector, coefficients))
		{
			static qreal value;
			value = 0;
			count = coefficients.size();
			for(i = 0; i < count; ++i)
			{
				value += coefficients[count - i - 1] * mPowers[i];
			}
			setNoise(qAbs(value - samples[size]));
		}
		else
		{
			setNoise(0);
		}

		samples.removeFirst();
	}
}

ViPredictionNoiseDetector* ViPredictionNoiseDetector::clone()
{
	return new ViPredictionNoiseDetector(*this);
}

#ifdef __cplusplus
extern "C"
{
#endif

ViPredictionNoiseDetector* create()
{
	return new ViPredictionNoiseDetector();
}

#ifdef __cplusplus
}
#endif
