#include <vipredictionnoisedetector.h>
#include <visystemsolver.h>

#define WINDOW_SIZE 128

ViPredictionNoiseDetector::ViPredictionNoiseDetector(const int &degree)
	: ViNoiseDetector()
{
	setDegree(degree);
	setOffset(WINDOW_SIZE);
}

QString ViPredictionNoiseDetector::name(QString replace, bool spaced)
{
	QString n = ViNoiseDetector::name(replace, spaced);
	if(spaced) n += " ";
	return n + QString::number(mDegree);
}

void ViPredictionNoiseDetector::setDegree(const int &degree)
{
	mDegree = degree;
	int i, j, size = WINDOW_SIZE;
	mMatrix = ViMatrix(size, mDegree + 1);
	for(i = 0; i < size; ++i)
	{
		mMatrix[i][0] = 1;
		for(j = 1; j <= mDegree; ++j)
		{
			mMatrix[i][j] = qPow(i, j);
		}
		mPowers.append(qPow(WINDOW_SIZE, mDegree - i));
	}
}

int ViPredictionNoiseDetector::degree()
{
	return mDegree;
}

void ViPredictionNoiseDetector::calculateNoise(QQueue<qreal> &samples)
{
	static int i, count;
	ViVector vector(WINDOW_SIZE);
	static ViVector coefficients;

	while(samples.size() > WINDOW_SIZE)
	{
		for(i = 0; i < WINDOW_SIZE; ++i)
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
				//value += coefficients[count - i - 1] * qPow(size, mDegree - i);
			}
			setNoise(qAbs(value - samples[WINDOW_SIZE]));
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
