#include <vihermitepredictor.h>
#include <vidifferentiater.h>
#include <vilogger.h>

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
		LOG("Invalid parameter for this predictor.", QtCriticalMsg);
		exit(-1);
	}
}


bool ViHermitePredictor::predict(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount)
{
	// http://www.math.usm.edu/lambers/mat772/fall10/lecture6.pdf
	// http://bruce-shapiro.com/math481A/notes/19-Hermite-interpolation.pdf
	// http://www.phys.ufl.edu/~coldwell/wsteve/FDERS/The%20Lagrange%20Polynomial.htm

	static int i, j, m, k, x;
	static qreal result;
	static qreal totalDevider; // qreal, since we devide

	qreal scaledX, scaledI, scaledM, scaledK;
	qreal value, lagrangeSquared, lagrangeFirst, lagrangeFirst1;

	totalDevider = size + predictionCount - 1;

	bool error;



	for(j = 0; j < predictionCount; ++j)
	{
		x = size + j;
		scaledX = x / totalDevider;

		result = 0;
		for(i = 0; i < size; ++i)
		{
			scaledI = i / totalDevider;

			// Lagrange
			lagrangeSquared = 1;
			for(m = 0; m < size; ++m)
			{
				if(i != m)
				{
					scaledM = m / totalDevider;
					lagrangeSquared *= (scaledX - scaledM) / (scaledI - scaledM);
				}
			}
			lagrangeSquared *= lagrangeSquared;




			/*if(i==0)
			{
				lagrangeSquared = (scaledX - (1/totalDevider)) / (0 - (1/totalDevider));
				lagrangeFirst = 1 / (0 - (1/totalDevider));
			}
			else if(i==1)
			{
				lagrangeSquared = (scaledX - 0) / ((1/totalDevider) - 0);
				lagrangeFirst = 1 / ((1/totalDevider) - 0);
			}*/



			// Lagrange first derivative
			lagrangeFirst = 0;
			for(m = 0; m < size; ++m)
			{
				if(i != m)
				{
					scaledM = m / totalDevider;
					lagrangeFirst += 1 /(scaledI-scaledM);
				}
			}




			/*for(k = 0; k < size; ++k)
			{
				if(i != k)
				{
					scaledK = k / totalDevider;
					lagrangeFirst1 = 1 / (scaledI - scaledK);
					for(m = 0; m < size; ++m)
					{
						if(i != m)
						{
							scaledM = m / totalDevider;
							lagrangeFirst1 *= (scaledX - scaledM) / (scaledI - scaledM);
						}
					}
					lagrangeFirst += lagrangeFirst1;
				}
			}*/

			// Normal polynomial (first part)
			value = lagrangeSquared * (1 - (2 * lagrangeFirst * (scaledX - scaledI)));
			result += samples[i] * value;

			// First derivative (second part)
			value = lagrangeSquared  * (scaledX - scaledI);
			result += ViDifferentiater::derivative(1, samples, size, x, error) * value;
		}
		predictedSamples[j] = result;
	}

	return true;
}

qreal ViHermitePredictor::lagrange(const qreal *samples, const int &size, const int &x, const qreal &scaler)
{
	static int i, j;
	static qreal result, value, scaledI, scaledJ, scaledX;

	scaledX = x / scaler;
	result = 0;

	for(i = 0; i < size; ++i)
	{
		scaledI = i / scaler;

		value = 1;
		for(j = 0; j < size; ++j)
		{
			if(i != j)
			{
				scaledJ = j / scaler;
				value *= (scaledX - scaledJ) / (scaledI - scaledJ);
			}
		}

		result += samples[i] * value;
	}

	return result;
}

qreal ViHermitePredictor::lagrangeFirstDerivative(const qreal *samples, const int &size, const int &x, const qreal &scaler)
{
	static int i, j;
	static qreal result, value, scaledI, scaledJ, scaledX;

	scaledX = x / scaler;
	result = 0;

	for(i = 0; i < size; ++i)
	{
		scaledI = i / scaler;

		value = 1;
		for(j = 0; j < size; ++j)
		{
			if(i != j)
			{
				scaledJ = j / scaler;
				value *= (scaledX - scaledJ) / (scaledI - scaledJ);
			}
		}

		result += samples[i] * value;
	}

	return result;
}

bool ViHermitePredictor::estimate(const int &degree, ViVector &coefficients, const qreal *samples, const int &size, const qreal &scaling)
{

}

