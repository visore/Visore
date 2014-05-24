#include <vihermitepredictor.h>
#include <visystemsolver.h>
#include <vilogger.h>
#include <vistandardpolynomial.h>

#define DEFAULT_DEGREE 3

ViHermitePredictor::ViHermitePredictor(ViPolynomial *polynomial, const DerivativeMode &mode, const Estimation &estimation)
	: ViModelPredictor(1, estimation)
{
	mPolynomial = polynomial;
	mPolynomial->setDerivatives(1);

	setDerivativeMode(mode);
	setDegree(DEFAULT_DEGREE);
	addParameterName("Window Size");
	addParameterName("Degree");
}

ViHermitePredictor::ViHermitePredictor(const ViHermitePredictor &other)
	: ViModelPredictor(other)
{
}

ViHermitePredictor::~ViHermitePredictor()
{
}

void ViHermitePredictor::setDerivativeMode(const DerivativeMode &mode)
{
	mDerivativeMode = mode;
	if(mDerivativeMode == Outgoing) estimatePointer = &ViHermitePredictor::estimateModelOutgoing;
	else if(mDerivativeMode == Ingoing) estimatePointer = &ViHermitePredictor::estimateModelIngoing;
	else if(mDerivativeMode == Average) estimatePointer = &ViHermitePredictor::estimateModelAverage;
}

void ViHermitePredictor::setDegree(const int &degree, const int &degreeIndex)
{
	ViModelPredictor::setDegree(degree, degreeIndex);
	mPolynomial->setDegree(degree);
}

void ViHermitePredictor::setParameter(const int &number, const qreal &value)
{
	if(number == 0) setWindowSize(value);
	else if(number == 1) setDegree(value);
	else
	{
		LOG("Invalid parameter for this predictor.", QtCriticalMsg);
		exit(-1);
	}
}

bool ViHermitePredictor::estimateModel(const int &degree, ViVector &coefficients, const qreal *samples, const int &size)
{
	(this->*estimatePointer)(degree, coefficients, samples, size);
	/*qreal derivatives[size];
	calculateFirstDerivative(samples, size, derivatives);

	ViVector vector(size, samples, size, derivatives);
	ViMatrix matrix(mPolynomial->get(degree, 0), size, mPolynomial->get(degree, 1), size);

	return ViSystemSolver::solve(matrix, vector, coefficients);*/
}

bool ViHermitePredictor::estimateModelOutgoing(const int &degree, ViVector &coefficients, const qreal *samples, const int &size)
{
	/*int i, newSize = size - 1;
	qreal derivatives[newSize];

	for(i = 0; i < newSize; ++i) derivatives[i] = samples[i + 1] - samples[i];

	ViVector vector(size, samples, newSize, derivatives);
	ViMatrix matrix(mPolynomial->get(degree, 0), size, mPolynomial->get(degree, 1), newSize);

	return ViSystemSolver::solve(matrix, vector, coefficients);*/

	int i, newSize = size - 1;
		qreal derivatives[size];

		for(i = 0; i < newSize; ++i) derivatives[i] = samples[i + 1] - samples[i];
		derivatives[newSize]=0;

		ViVector vector(size, samples, size, derivatives);
		ViMatrix matrix(mPolynomial->get(degree, 0), size, mPolynomial->get(degree, 1), size);

		return ViSystemSolver::solve(matrix, vector, coefficients);
}

bool ViHermitePredictor::estimateModelIngoing(const int &degree, ViVector &coefficients, const qreal *samples, const int &size)
{
	int i, newSize = size - 1;
	qreal derivatives[newSize];

	for(i = 0; i < newSize; ++i) derivatives[i] = samples[i + 1] - samples[i];

	ViVector vector(size, samples, newSize, derivatives);
	ViMatrix matrix(mPolynomial->get(degree, 0), size, mPolynomial->getOffset(degree, 1, 1), newSize);

	return ViSystemSolver::solve(matrix, vector, coefficients);
}

bool ViHermitePredictor::estimateModelAverage(const int &degree, ViVector &coefficients, const qreal *samples, const int &size)
{
	int i, newSize = size - 2;
	qreal derivatives[newSize];

	for(i = 0; i < newSize; ++i) derivatives[i] = ((samples[i + 1] - samples[i]) + (samples[i + 2] - samples[i + 1])) / 2;

	ViVector vector(size, samples, newSize, derivatives);
	ViMatrix matrix(mPolynomial->get(degree, 0), size, mPolynomial->getOffset(degree, 1, 1), newSize);

	return ViSystemSolver::solve(matrix, vector, coefficients);
}

void ViHermitePredictor::predictModel(const int &degree, const ViVector &coefficients, qreal *prediction, const int &size, const int &start)
{
	// Do not use the derivative polynomials here, since the calculate the slopes and the the y-values
	static int i;
	for(i = 0; i < size; ++i) mPolynomial->solve(degree, start + i, coefficients, prediction[i]);
}

void ViHermitePredictor::calculateFirstDerivative(const qreal *samples, const int &size, qreal *derivatives)
{
	int i, end;
	end = size - 1;

	//for(i = 1; i < end; ++i) derivatives[i-1] = ((samples[i] - samples[i - 1]) + (samples[i + 1] - samples[i])) / 2;*/

	// Set first and last derivative to 0
	/*derivatives[0] = ((samples[1] - samples[0]) + (samples[0]-samples[0]))/2;
	derivatives[end] = ((samples[end] - samples[end-1]) + (samples[end]-samples[end]))/2;
	for(i = 1; i < end; ++i) derivatives[i] = ((samples[i] - samples[i - 1]) + (samples[i + 1] - samples[i])) / 2;*/

	//for(i = 0; i <= end; ++i) derivatives[i] = 0;

	derivatives[end] = 0;
	for(i = 0; i < end; ++i) derivatives[i] = samples[i+1] - samples[i];
}
