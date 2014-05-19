#include <vipolynomialpredictor.h>
#include <visystemsolver.h>

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

void ViPolynomialPredictor::setParameters(const qreal &parameter1, const qreal &parameter2)
{
	setWindowSize(parameter1);
	setDegree(parameter2);
}

void ViPolynomialPredictor::predict(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount)
{
	static int i, j, x, count;
	static qreal value;

	ViMatrix matrix(size, mDegree + 1);
	ViVector vector(size);

	for(i = 0; i < size; ++i)
	{
		vector[i] = samples[i];
		matrix[i][0] = 1;
		for(j = 1; j <= mDegree; ++j)
		{
			matrix[i][j] = qPow(i, j);
		}
	}

	static ViVector coefficients;
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
