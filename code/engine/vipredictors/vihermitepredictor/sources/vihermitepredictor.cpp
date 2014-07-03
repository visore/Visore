#include <vihermitepredictor.h>
#include <vidifferentiater.h>
#include <vilogger.h>


#define DEFAULT_DEGREE 1

ViHermitePredictor::ViHermitePredictor()
	: ViPredictor()
{
	addParameterName("Window Size");
}

ViHermitePredictor::ViHermitePredictor(const ViHermitePredictor &other)
	: ViPredictor(other)
{
}

ViHermitePredictor::~ViHermitePredictor()
{
}

void ViHermitePredictor::setParameter(const int &number, const qreal &value)
{
	if(number == 0) setWindowSize(value);
	else
	{
		LOG("Invalid parameter for this Predictor.", QtCriticalMsg);
		exit(-1);
	}
}

bool ViHermitePredictor::validParameters()
{
	return mWindowSize > 1;
}

qreal ViHermitePredictor::calculateLagrange(const qreal *x, const int &size, const qreal &theX, const int &j)
{
	static int m;
	static qreal product;

	product = 1;
	for(m = 0; m < size; ++m)
	{
		if(m != j) product *= (theX - x[m]) / (x[j] - x[m]);
	}
	return product;
}

qreal ViHermitePredictor::calculateLagrangeDerivative1(const qreal *x, const int &size, const qreal &theX, const int &j)
{
	//http://www.phys.ufl.edu/~coldwell/wsteve/FDERS/The%20Lagrange%20Polynomial.htm
	//http://www.phys.ufl.edu/~coldwell/wsteve/FDERS/The%20Lagrange%20Polynomial_files/eq0006MP.gif
	//http://www2.math.umd.edu/~dlevy/classes/amsc466/lecture-notes/differentiation-chap.pdf

	static int l, m;
	static qreal result, product;

	result = 0;
	for(l = 0; l < size; ++l)
	{
		if(l != j)
		{
			product = 1 / (x[j] - x[l]);
			for(m = 0; m < size; ++m)
			{
				if(m != l && m != j) product *= (theX - x[m]) / (x[j] - x[m]);
			}
			result += product;
		}
	}
	return result;
}

bool ViHermitePredictor::predict(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, ViError *modelError, const int &channel)
{
	static int i;
	static qreal scaling;

	scaling = size - 1;

	qreal x[size];

	for(i = 0; i < size; ++i) x[i] = i / scaling;

	qreal derivatives[size];
	ViDifferentiater::derivative(1, samples, size, derivatives);

	calculate(x, samples, derivatives, size, predictedSamples, predictionCount, size, scaling);

	if(modelError != NULL)
	{
		qreal model[size];
		calculate(x, samples, derivatives, size, model, size, 0, scaling);
		modelError->add(model, samples, size);
	}

	return true;
}

void ViHermitePredictor::calculate(const qreal *x, const qreal *y, const qreal *derivatives, const int &size, qreal *output, const int &outputSize, const int &startX, const qreal &scaling)
{
	// http://www.math.usm.edu/lambers/mat772/fall10/lecture6.pdf
	// http://bruce-shapiro.com/math481A/notes/19-Hermite-interpolation.pdf
	// http://www.phys.ufl.edu/~coldwell/wsteve/FDERS/The%20Lagrange%20Polynomial.htm

	static int i, j;
	static qreal outputX, h1, h2, lagrange, lagrangeSquared, result, derivative;

	for(i = 0; i < outputSize; ++i)
	{
		result = 0;
		outputX = (startX + i) / scaling;

		for(j = 0; j < size; ++j)
		{
			// Calculate Lagrange base polynomial
			lagrange = calculateLagrange(x, size, outputX, j);
			lagrangeSquared = lagrange * lagrange;

			// Calculate Lagrange base polynomial derivative
			derivative = calculateLagrangeDerivative1(x, size, x[j], j);

			h1 = (1 - (2 * (outputX - x[j]) * derivative)) * lagrangeSquared;
			h2 = (outputX - x[j]) * lagrangeSquared;

			result += (y[j] * h1) + (derivatives[j] * h2);
		}
		output[i] = result;
	}
}

ViHermitePredictor* ViHermitePredictor::clone()
{
	return new ViHermitePredictor(*this);
}
