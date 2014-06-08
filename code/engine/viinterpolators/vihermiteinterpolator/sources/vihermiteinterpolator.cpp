#include <vihermiteinterpolator.h>
#include <vieigen.h>
#include <vidifferentiater.h>

#define DEFAULT_DEGREE 1

ViHermiteInterpolator::ViHermiteInterpolator()
	: ViInterpolator()
{
	setDegree(DEFAULT_DEGREE);
	addParameterName("Window Size");
	addParameterName("Degree");
}

ViHermiteInterpolator::ViHermiteInterpolator(const ViHermiteInterpolator &other)
	: ViInterpolator(other)
{
	mDegree = other.mDegree;
}

ViHermiteInterpolator::~ViHermiteInterpolator()
{
}

void ViHermiteInterpolator::setDegree(const int &degree)
{
	mDegree = degree;
}

void ViHermiteInterpolator::setParameter(const int &number, const qreal &value)
{
	if(number == 0) setWindowSize(value);
	else if(number == 1) setDegree(value);
	else
	{
		LOG("Invalid parameter for this interpolator.", QtCriticalMsg);
		exit(-1);
	}
}

bool ViHermiteInterpolator::validParameters()
{
	return mDegree > 0 && mWindowSize > 1;
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

bool ViHermiteInterpolator::interpolate(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize)
{
	// http://www.math.usm.edu/lambers/mat772/fall10/lecture6.pdf
	// http://bruce-shapiro.com/math481A/notes/19-Hermite-interpolation.pdf
	// http://www.phys.ufl.edu/~coldwell/wsteve/FDERS/The%20Lagrange%20Polynomial.htm

	/*qreal x[] = {0,2,3,4};
	cout<<calculateLagrange(x,4,1,0)<<endl;
	cout<<calculateLagrangeDerivative1(x,4,1,0)<<endl;
	exit(-1);*/


/*
	int size = 8;
	qreal outputX, h1, h2;
	qreal x[]={0, 1,2,3,6,7,8,9};
	qreal y[]={1,2,3,4,4,3,2,1};

	qreal y1[]={1,2,3,4};
	qreal y2[]={4,3,2,1};


qreal d[size];
ViDifferentiater::derivative(1, y1, 4, y2, 4, 2, d);

	for(int i = 0; i < 2; ++i)
	{
		qreal result = 0;

		outputX = ((size/2))+i;

		for(int j = 0; j < size; ++j)
		{
			// Calculate Lagrange base polynomial
			qreal lagrange = calculateLagrange(x, size, outputX, j);
			qreal lagrangeSquared = lagrange * lagrange;

			// Calculate Lagrange base polynomial derivative
			qreal derivative = calculateLagrangeDerivative1(x, size, x[j], j);

			h1 = (1 - (2 * (outputX - x[j]) * derivative)) * lagrangeSquared;
			h2 = (outputX - x[j]) * lagrangeSquared;

			//cout<<j<<"\t\t"<<ViDifferentiate::derivative(1, y, j)<<endl;

			result += (y[j] * h1) + (d[j] * h2);
			cout<<"ppp: "<<d[j]<<endl;
		}//
		cout<<result<<endl;
		outputSamples[i] = result;
	}
exit(-1);






*/

	int size = leftSize + rightSize;
		qreal outputX, h1, h2;
		qreal scaling = size - 1;

		qreal x[size];
		qreal y[size];

		for(int i = 0; i < leftSize; ++i)
		{
			x[i] = i / scaling;
			y[i] = leftSamples[i];
		}
		for(int i = 0; i < rightSize; ++i)
		{
			x[i + leftSize] = (leftSize + outputSize + i) / scaling;
			y[i + leftSize] = rightSamples[i];
		}

		qreal derivatives[size];
		ViDifferentiater::derivative(1, leftSamples, leftSize, rightSamples, rightSize, outputSize, derivatives);

		for(int i = 0; i < outputSize; ++i)
		{
			qreal result = 0;

			outputX = (leftSize + i) / scaling;

			for(int j = 0; j < size; ++j)
			{
				// Calculate Lagrange base polynomial
				qreal lagrange = calculateLagrange(x, size, outputX, j);
				qreal lagrangeSquared = lagrange * lagrange;

				// Calculate Lagrange base polynomial derivative
				qreal derivative = calculateLagrangeDerivative1(x, size, x[j], j);

				h1 = (1 - (2 * (outputX - x[j]) * derivative)) * lagrangeSquared;
				h2 = (outputX - x[j]) * lagrangeSquared;

				result += (y[j] * h1) + (derivatives[j] * h2);
			}
			outputSamples[i] = result;
		}










/*



	int size = leftSize + rightSize;
	qreal outputX, h1, h2;
	qreal scaling = size - 1;

	qreal x[size];
	qreal y[size];

	for(int i = 0; i < leftSize; ++i)
	{
		x[i] = i / scaling;
		y[i] = leftSamples[i];
	}
	for(int i = 0; i < rightSize; ++i)
	{
		x[i + leftSize] = (leftSize + outputSize + i) / scaling;
		y[i + leftSize] = rightSamples[i];
	}

	qreal derivatives[size];
	ViDifferentiater::derivative(1, leftSamples, leftSize, rightSamples, rightSize, outputSize, derivatives);

	for(int i = 0; i < outputSize; ++i)
	{
		qreal result = 0;

		outputX = (leftSize + i) / scaling;

		for(int j = 0; j < size; ++j)
		{
			// Calculate Lagrange base polynomial
			qreal lagrange = calculateLagrange(x, size, outputX, j);
			qreal lagrangeSquared = lagrange * lagrange;

			// Calculate Lagrange base polynomial derivative
			qreal derivative = calculateLagrangeDerivative1(x, size, x[j], j);

			h1 = (1 - (2 * (outputX - x[j]) * derivative)) * lagrangeSquared;
			h2 = (outputX - x[j]) * lagrangeSquared;

			result += (y[j] * h1) + (derivatives[j] * h2);
		}
		outputSamples[i] = result;
	}

*/





	/*static int i, j, x, newLeftSize, sampleCount, exponent;

	// We need outgoing slopes, so we can't use the first or last point (used to calculate the second and second-last slope)
	newLeftSize = leftSize - 1;
	++leftSamples;
	if(newLeftSize < 1 || rightSize < 2) return false;

	sampleCount = newLeftSize + rightSize;
	ViVector vector(sampleCount * 2);
	ViMatrix matrix(sampleCount * 2, mDegree + 1);

	for(i = 0; i < sampleCount; ++i)
	{
		if(i < newLeftSize)
		{
			// Fill the first half of the vector with the y-values
			vector[i] = leftSamples[i];
			// Fill the second half of the vector with the outgoing slopes
			vector[i + sampleCount] = leftSamples[i] - leftSamples[i - 1];
			// For the matrix
			x = i;
		}
		else
		{
			x = i - newLeftSize;
			// Fill the first half of the vector with the y-values
			vector[i] = rightSamples[x];
			// Fill the second half of the vector with the outgoing slopes
			vector[i + sampleCount] = rightSamples[x + 1] - rightSamples[x];
			// For the matrix
			x = i + outputSize;
		}

		for(j = 0; j < mDegree; ++j)
		{
			exponent = mDegree - j;
			// Fill the first half of the matrix with the Hermite polynomials constructed from the x-values
			matrix[i][j] = qPow(x, exponent);
			// Fill the second half of the matrix with the derivitaves
			matrix[i + sampleCount][j] = exponent * qPow(x, exponent - 1);
		}
		matrix[i][mDegree] = 1;
	}

	static ViVector coefficients;
	if(ViSystemSolver::solve(matrix, vector, coefficients))
	{
		static qreal value;
		static int count;
		count = coefficients.size();
		for(i = 0; i < outputSize; ++i)
		{
			x = newLeftSize + i;
			value = 0;
			for(j = 0; j < count; ++j)
			{
				value += coefficients[j] * qPow(x, mDegree - j);
			}
			outputSamples[i] = value;
		}
		return true;
	}
	return false;*/
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

