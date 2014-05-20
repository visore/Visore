#include <vihermitepredictor.h>
#include <visystemsolver.h>
#include <vilogger.h>

ViHermitePredictor::ViHermitePredictor()
	: ViDegreePredictor()
{
	addParameterName("Window Size");
	addParameterName("Degree");
}

ViHermitePredictor::ViHermitePredictor(const ViHermitePredictor &other)
	: ViDegreePredictor(other)
{
}

ViHermitePredictor::~ViHermitePredictor()
{
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

void ViHermitePredictor::predict(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount)
{
	static int i, j, x, newSize, doubleSize, exponent;
	static ViVector coefficients;

	// We need outgoing slopes, so we can't use the first or last point (used to calculate the second and second-last slope)
	newSize = size - 1;
	doubleSize = newSize * 2;
	ViVector vector(doubleSize);
	ViMatrix matrix(doubleSize, mDegree + 1);

	for(i = 0; i < newSize; ++i)
	{
		vector[i] = samples[i];
		vector[i + newSize] = samples[i] - samples[i - 1]; // Fill the second half of the vector with the outgoing slopes
		x = i; // For the matrix

		for(j = 0; j < mDegree; ++j)
		{
			exponent = mDegree - j;
			matrix[i][j] = qPow(x, exponent); // Fill the first half of the matrix with the Hermite polynomials constructed from the x-values
			matrix[i + newSize][j] = exponent * qPow(x, exponent - 1); // Fill the second half of the matrix with the derivitaves
		}
		matrix[i][mDegree] = 1;
	}

	if(ViSystemSolver::solve(matrix, vector, coefficients))
	{
		static qreal value;
		static int count;
		count = coefficients.size();
		for(i = 0; i < predictionCount; ++i)
		{
			x = newSize + i;
			value = 0;
			for(j = 0; j < count; ++j) value += coefficients[j] * qPow(x, mDegree - j);
			predictedSamples[i] = value;
		}
	}
	else
	{
		for(i = 0; i < predictionCount; ++i) predictedSamples[i] = 0;
	}
}
