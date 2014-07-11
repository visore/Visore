#include <vihermiteinterpolator.h>
#include <vieigen.h>
#include <vidifferentiater.h>

ViHermiteInterpolator::ViHermiteInterpolator()
	: ViInterpolator()
{
	addParameterName("Window Size");
}

ViHermiteInterpolator::ViHermiteInterpolator(const ViHermiteInterpolator &other)
	: ViInterpolator(other)
{
}

ViHermiteInterpolator::~ViHermiteInterpolator()
{
}

void ViHermiteInterpolator::setParameter(const int &number, const qreal &value)
{
	if(number == 0) setWindowSize(value);
	else
	{
		LOG("Invalid parameter for this interpolator.", QtCriticalMsg);
		exit(-1);
	}
}

bool ViHermiteInterpolator::validParameters()
{
	return mWindowSize > 1;
}

qreal ViHermiteInterpolator::calculateLagrange(const qreal *x, const int &size, const qreal &theX, const int &j)
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

qreal ViHermiteInterpolator::calculateLagrangeDerivative1(const qreal *x, const int &size, const qreal &theX, const int &j)
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

bool ViHermiteInterpolator::interpolate(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize, ViError *error, const int &channel)
{
	static int i, size;
	static qreal scaling;

	size = leftSize + rightSize;
	scaling = size - 1;

	qreal x[size];
	qreal y[size];

	for(i = 0; i < leftSize; ++i)
	{
		x[i] = i / scaling;
		y[i] = leftSamples[i];
	}
	for(i = 0; i < rightSize; ++i)
	{
		x[i + leftSize] = (leftSize + outputSize + i) / scaling;
		y[i + leftSize] = rightSamples[i];
	}

	qreal derivatives[size];
	ViDifferentiater::derivative(1, leftSamples, leftSize, rightSamples, rightSize, outputSize, derivatives);

	calculate(x, y, derivatives, size, outputSamples, outputSize, leftSize, scaling);

	if(error != NULL)
	{
		qreal modelLeft[leftSize];
		calculate(x, y, derivatives, size, modelLeft, leftSize, 0, scaling);
		error->add(modelLeft, leftSamples, leftSize);

		qreal modelRight[rightSize];
		calculate(x, y, derivatives, size, modelRight, rightSize, leftSize + outputSize, scaling);
		error->add(modelRight, rightSamples, rightSize);
	}

	return true;
}

void ViHermiteInterpolator::calculate(const qreal *x, const qreal *y, const qreal *derivatives, const int &size, qreal *output, const int &outputSize, const int &startX, const qreal &scaling)
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

ViHermiteInterpolator* ViHermiteInterpolator::clone()
{
	return new ViHermiteInterpolator(*this);
}

#ifdef __cplusplus
extern "C"
{
#endif

ViHermiteInterpolator* create()
{
	return new ViHermiteInterpolator();
}

#ifdef __cplusplus
}
#endif

