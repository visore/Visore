#include <vipolynomialpredictor.h>
#include <visystemsolver.h>
#include <vilogger.h>

#define DEFAULT_DEGREE 3

ViPolynomialPredictor::ViPolynomialPredictor(const Estimation &estimation)
	: ViModelPredictor(1, estimation)
{
	setDegree(DEFAULT_DEGREE);
	addParameterName("Window Size");
	addParameterName("Degree");
}

ViPolynomialPredictor::ViPolynomialPredictor(const ViPolynomialPredictor &other)
	: ViModelPredictor(other)
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

bool ViPolynomialPredictor::estimateModel(const int &degree, ViVector &coefficients, const qreal *samples, const int &size)
{
	static int i, j;

	ViMatrix matrix(size, degree + 1);
	ViVector vector(size);

	for(i = 0; i < size; ++i)
	{
		vector[i] = samples[i];
		matrix[i][0] = 1;
		for(j = 1; j <= degree; ++j) matrix[i][j] = qPow(i, j);
	}

	return ViSystemSolver::solve(matrix, vector, coefficients);
}

void ViPolynomialPredictor::predictModel(const int &degree, const ViVector &coefficients, qreal *prediction, const int &size, const int &start)
{
	static int i, j, x, count;
	static qreal value;

	count = coefficients.size();
	for(i = 0; i < size; ++i)
	{
		x = i + start;
		value = 0;
		for(j = 0; j < count; ++j) value += coefficients[count - j - 1] * qPow(x, degree - j);
		prediction[i] = value;
	}
}
