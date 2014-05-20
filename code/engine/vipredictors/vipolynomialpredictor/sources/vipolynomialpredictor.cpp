#include <vipolynomialpredictor.h>
#include <visystemsolver.h>
#include <vilogger.h>

ViPolynomialPredictor::ViPolynomialPredictor()
	: ViDegreePredictor()
{
	addParameterName("Window Size");
	addParameterName("Degree");
}

ViPolynomialPredictor::ViPolynomialPredictor(const ViPolynomialPredictor &other)
	: ViDegreePredictor(other)
{
}

ViPolynomialPredictor::~ViPolynomialPredictor()
{
}

void ViPolynomialPredictor::setParameter(const int &number, const qreal &value)
{
	if(number == 0) setWindowSize(value);
	else if(number == 1) setDegree(value);
	else
	{
		LOG("Invalid parameter for this predictor.", QtCriticalMsg);
		exit(-1);
	}
}

void ViPolynomialPredictor::predict(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount)
{
	static int i, j, x, count;
	static qreal value;
	static ViVector coefficients;

	ViMatrix matrix(size, mDegree + 1);
	ViVector vector(size);

	for(i = 0; i < size; ++i)
	{
		vector[i] = samples[i];
		matrix[i][0] = 1;
		for(j = 1; j <= mDegree; ++j) matrix[i][j] = qPow(i, j);
	}

	if(ViSystemSolver::solve(matrix, vector, coefficients))
	{
		count = coefficients.size();
		for(i = 0; i < predictionCount; ++i)
		{
			x = size + i;
			value = 0;
			for(j = 0; j < count; ++j) value += coefficients[count - j - 1] * qPow(x, mDegree - j);
			predictedSamples[i] = value;
		}
	}
	else
	{
		for(i = 0; i < predictionCount; ++i) predictedSamples[i] = 0;
	}
}
