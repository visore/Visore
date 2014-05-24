#include <vipolynomialpredictor.h>
#include <visystemsolver.h>
#include <vilogger.h>

#define DEFAULT_DEGREE 3

ViPolynomialPredictor::ViPolynomialPredictor(ViPolynomial *polynomial, const Estimation &estimation)
	: ViModelPredictor(1, estimation)
{
	mPolynomial = polynomial;
	if(mPolynomial == NULL)
	{
		LOG("The polynomial cannot be NULL.", QtCriticalMsg);
		exit(-1);
	}
	setDegree(DEFAULT_DEGREE);
	addParameterName("Window Size");
	addParameterName("Degree");
}

ViPolynomialPredictor::ViPolynomialPredictor(const ViPolynomialPredictor &other)
	: ViModelPredictor(other)
{
	mPolynomial = other.mPolynomial->clone();
}

ViPolynomialPredictor::~ViPolynomialPredictor()
{
	delete mPolynomial;
}

void ViPolynomialPredictor::setDegree(const int &degree, const int &degreeIndex)
{
	ViModelPredictor::setDegree(degree, degreeIndex);
	mPolynomial->setDegree(degree);
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
	ViMatrix matrix(mPolynomial->get(degree), size);
	ViVector vector(size, samples);
	return ViSystemSolver::solve(matrix, vector, coefficients);
}

void ViPolynomialPredictor::predictModel(const int &degree, const ViVector &coefficients, qreal *prediction, const int &size, const int &start)
{
	static int i;
	for(i = 0; i < size; ++i) mPolynomial->solve(degree, start + i, coefficients, prediction[i]);
}
