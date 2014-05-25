#include <vifourierpredictor.h>
#include <visystemsolver.h>
#include <vilogger.h>
#include <vidifferentiater.h>
#include <float.h>

#define DEFAULT_DEGREE 3
#define DEFAULT_DERIVATIVE 0

ViFourierPredictor::ViFourierPredictor(const Mode &mode, const Estimation &estimation)
	: ViPredictor()
{
	setPointers(mode, estimation);
	setDegree(DEFAULT_DEGREE);
	setDerivatives(DEFAULT_DERIVATIVE);

	addParameterName("Window Size");
	addParameterName("Degree");
	addParameterName("Derivatives");
}

ViFourierPredictor::ViFourierPredictor(const ViFourierPredictor &other)
	: ViPredictor(other)
{
	setPointers(other.mMode, other.mEstimation);
	setDegree(other.mDegree);
	setDerivatives(other.mDerivatives);
}

ViFourierPredictor::~ViFourierPredictor()
{
}

void ViFourierPredictor::setDegree(const int &degree)
{
	mDegree = degree;
	mBestDegrees.clear();
	mBestDegrees.resize(degree);
	mBestDegrees.fill(0);
	if(mDerivatives > mDegree) LOG("Can't use these derivitives with the given degree. Setting derivatives equal to degree.", QtCriticalMsg);
}

void ViFourierPredictor::setDerivatives(const int &derivatives)
{
	mDerivatives = derivatives;
	if(mDerivatives > mDegree) LOG("Can't use these derivitives with the given degree. Setting derivatives equal to degree.", QtCriticalMsg);
}

void ViFourierPredictor::setParameter(const int &number, const qreal &value)
{
	if(number == 0) setWindowSize(value);
	else if(number == 1) setDegree(value);
	else if(number == 2) setDerivatives(value);
	else
	{
		LOG("Invalid parameter for this predictor.", QtCriticalMsg);
		exit(-1);
	}
}

void ViFourierPredictor::setPointers(const Mode &mode, const Estimation &estimation)
{
	mMode = mode;
	if(mMode == Normal)
	{
		//predictModelPointer = &ViFourierPredictor::predictModelNormal;
		//estimateModelPointer = &ViFourierPredictor::estimateModelNormal;
	}
	else if(mMode == Osculating)
	{
		predictModelPointer = &ViFourierPredictor::predictModelOsculating;
		estimateModelPointer = &ViFourierPredictor::estimateModelOsculating;
	}
	else if(mMode == Splines)
	{
		//predictModelPointer = &ViFourierPredictor::predictModelSplines;
		//estimateModelPointer = &ViFourierPredictor::estimateModelSplines;
	}

	mEstimation = estimation;
	if(mEstimation == Fixed) predictPointer = &ViFourierPredictor::predictFixed;
	else if(mEstimation == Best) predictPointer = &ViFourierPredictor::predictBest;
}

void ViFourierPredictor::predict(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount)
{
	// Scaling by (size - 1) to get the x-values in [0,1]. Otherwise working with 1, 2, 3, 4, ... will result in sin(PI*x) to always be 0.
	// Multiply by Pi, since we wok with sin and cos
	(this->*predictPointer)(samples, size, predictedSamples, predictionCount, M_PI / (size - 1));
}

void ViFourierPredictor::predictFixed(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, const qreal &scaling)
{
	static int i;
	static ViVector coefficients;
	if((this->*estimateModelPointer)(mDegree, coefficients, samples, size, scaling)) (this->*predictModelPointer)(mDegree, coefficients, predictedSamples, predictionCount, size, scaling);
	else for(i = 0; i < predictionCount; ++i) predictedSamples[i] = 0;
}

void ViFourierPredictor::predictBest(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, const qreal &scaling)
{
	static int i;
	static ViVector currentCoefficients, bestCoefficients;
	static qreal currentMse, bestMse;

	qreal prediction[size];
	bestMse = DBL_MAX;
	for(i = 1; i <= mDegree; ++i)
	{
		if((this->*estimateModelPointer)(i, currentCoefficients, samples, size, scaling))
		{
			(this->*predictModelPointer)(i, currentCoefficients, prediction, size, 0, scaling);
			currentMse = calculateMse(samples, prediction, size);
			if(currentMse < bestMse)
			{
				bestMse = currentMse;
				bestCoefficients = currentCoefficients;
			}
			else break;
		}
	}
	--i; // Important: last iteration will increase i before exiting the loop
	mBestDegrees[i - 1] += 1;
	(this->*predictModelPointer)(i, bestCoefficients, predictedSamples, predictionCount, size, scaling);
}
/*
bool ViFourierPredictor::estimateModelNormal(const int &degree, ViVector &coefficients, const qreal *samples, const int &size)
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

void ViFourierPredictor::predictModelNormal(const int &degree, const ViVector &coefficients, qreal *prediction, const int &size, const int &start)
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
}*/

bool ViFourierPredictor::estimateModelOsculating(const int &degree, ViVector &coefficients, const qreal *samples, const int &size, const qreal &scaling)
{
	static int i, j, offset, start, end, derivativeCount, totalDerivatives, totalSize;
	static qreal value1, value2;

	derivativeCount = size - 2; // Average, we don't have derivatives for the first and last point
	totalDerivatives = derivativeCount * mDerivatives;
	totalSize = size + totalDerivatives;

	ViMatrix matrix(totalSize, (2 * degree) + 1); // a0, a1 ... an and b1 ... bn
	ViVector vector(totalSize);

	for(i = 0; i < size; ++i)
	{
		vector[i] = samples[i];
		value1 = scaling * i;
		matrix[i][0] = 0.5;
		for(j = 1; j <= degree; ++j)
		{
			value2 = value1 * j;
			matrix[i][j] = qCos(value2);
			matrix[i][j + degree] = qSin(value2);
		}
	}

	// Determine for how many samples we cannot calculate a derivitate, and skip those equations.
	// Der1 and der2 must skip 1 sample at the start and 1 at the end, der3 and der4 skip 2, etc
	start = qCeil(mDerivatives / 2.0);

	end = size - start;
	for(i = 1; i <= mDerivatives; ++i)
	{
		for(j = start; j < end; ++j)
		{
			offset = size + (derivativeCount * (i - 1)) + (j - 1);
			bool error;
			vector[offset] = ViDifferentiate::derivative(i, samples, size, j, error);
			calculateDerivative(degree, scaling * j, matrix[offset], i);
		}
	}

	return ViSystemSolver::solve(matrix, vector, coefficients);
}

void ViFourierPredictor::predictModelOsculating(const int &degree, const ViVector &coefficients, qreal *prediction, const int &size, const int &start, const qreal &scaling)
{
	static int i, j;
	static qreal a0, value1, value2, value3;

	a0 = coefficients[0] / 2;
	for(i = 0; i < size; ++i)
	{
		value2 = scaling * (start + i);
		value1 = a0;
		for(j = 1; j <= degree; ++j)
		{
			value3 = value2 * j ;
			value1 += (coefficients[j] * qCos(value3)) + (coefficients[j + degree] * qSin(value3));
		}
		prediction[i] = value1;
	}
}
/*
bool ViFourierPredictor::estimateModelSplines(const int &degree, ViVector &coefficients, const qreal *samples, const int &size)
{

}

void ViFourierPredictor::predictModelSplines(const int &degree, const ViVector &coefficients, qreal *prediction, const int &size, const int &start)
{

}
*/
void ViFourierPredictor::calculateDerivative(const int &degree, const qreal &x, ViVector &row, const int &derivative)
{
	static int i;
	static qreal value1, value2;

	row[0] = 0;
	if(derivative % 4 == 0) // 4th, 8th, 12th, ... derivatives
	{
		for(i = 1; i <= degree; ++i)
		{
			value1 = x * i;
			value2 = qPow(i, derivative);
			row[i] = value2 * qCos(value1);
			row[i + degree] = value2 * qSin(value1);
		}
	}
	else if(derivative % 4 == 1) // 1st, 5th, 9th, ... derivatives
	{
		for(i = 1; i <= degree; ++i)
		{
			value1 = x * i;
			value2 = qPow(i, derivative);
			row[i] = value2 * qCos(value1);
			row[i + degree] = -value2 * qSin(value1);
		}
	}
	else if(derivative % 4 == 2) // 2nd, 6th, 10th, ... derivatives
	{
		for(i = 1; i <= degree; ++i)
		{
			value1 = x * i;
			value2 = -qPow(i, derivative);
			row[i] = value2 * qCos(value1);
			row[i + degree] = value2 * qSin(value1);
		}
	}
	else if(derivative % 4 == 3) // 3rd, 7th, 1th, ... derivatives
	{
		for(i = 1; i <= degree; ++i)
		{
			value1 = x * i;
			value2 = qPow(i, derivative);
			row[i] = -value2 * qCos(value1);
			row[i + degree] = value2 * qSin(value1);
		}
	}
}

qreal ViFourierPredictor::calculateMse(const qreal *observed, const qreal *predicted, const int &size)
{
	static int i;
	static qreal mse;

	mse = 0;
	for(i = 0; i < size; ++i) mse += qPow(predicted[i] - observed[i], 2);

	return mse / size;
}
