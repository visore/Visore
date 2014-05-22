#include <vifourierpredictor.h>
#include <visystemsolver.h>
#include <vilogger.h>

#define DEFAULT_DEGREE 3

ViFourierPredictor::ViFourierPredictor(const Estimation &estimation)
	: ViModelPredictor(1, estimation)
{
	addParameterName("Window Size");
	addParameterName("Degree");
}

ViFourierPredictor::ViFourierPredictor(const ViFourierPredictor &other)
	: ViModelPredictor(other)
{
}

ViFourierPredictor::~ViFourierPredictor()
{
}

void ViFourierPredictor::setParameter(const int &number, const qreal &value)
{
	if(number == 0) setWindowSize(value);
	else if(number == 1) setDegree(value);
	else
	{
		LOG("Invalid parameter for this predictor.", QtCriticalMsg);
		exit(-1);
	}
}

bool ViFourierPredictor::estimateModel(const int &degree, ViVector &coefficients, const qreal *samples, const int &size)
{
	// http://cant.ua.ac.be/sites/cant.ua.ac.be/files/courses/cscw/ratint/fourier.fausett.pdf
	// http://en.wikipedia.org/wiki/Trigonometric_polynomial

	static int i, j;
	static qreal value1, value2;

	ViMatrix matrix(size, (2 * degree) + 1); // a0, a1 ... an and b1 ... bn
	ViVector vector(size);

	for(i = 0; i < size; ++i)
	{
		vector[i] = samples[i];
		value1 = M_PI * i; // Multiple of Pi, since we work with sin and cos
		matrix[i][0] = 0.5;
		for(j = 1; j <= degree; ++j)
		{
			value2 = value1 * j;
			matrix[i][j] = qCos(value2);
			matrix[i][j + degree] = qSin(value2);
		}
	}

	return ViSystemSolver::solve(matrix, vector, coefficients);
}

void ViFourierPredictor::predictModel(const int &degree, const ViVector &coefficients, qreal *prediction, const int &size, const int &start)
{
	static int i, j;
	static qreal a0, value, value1, value2;

	a0 = coefficients[0] / 2;
	for(i = 0; i < size; ++i)
	{
		value1 = M_PI * (start + i); // Multiple of Pi, since we work with sin and cos
		value = a0;
		for(j = 1; j <= degree; ++j)
		{
			value2 = value1 * j ;
			value += (coefficients[j] * qCos(value2)) + (coefficients[j + degree] * qSin(value2));
		}
		prediction[i] = value;
	}
}
