#include <vihermitepredictor.h>
#include <visystemsolver.h>
#include <vilogger.h>

#define DEFAULT_DEGREE 3

ViHermitePredictor::ViHermitePredictor()
	: ViPredictor()
{
	setDegree(DEFAULT_DEGREE);

	addParameterName("Window Size");
	addParameterName("Degree");
}

ViHermitePredictor::ViHermitePredictor(const ViHermitePredictor &other)
	: ViPredictor(other)
{
	mDegree = other.mDegree;
}

ViHermitePredictor::~ViHermitePredictor()
{
}

void ViHermitePredictor::setDegree(const int &degree)
{
	mDegree = degree;
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
	ViMatrix matrix(doubleSize, mDegree *2);

	for(i = 0; i < newSize; ++i)
	{
		//vector[i] = samples[i];
		vector[i] = samples[i+1];
		vector[i + newSize] = samples[i + 1] - samples[i]; // Fill the second half of the vector with the outgoing slopes

		for(j = 0; j < mDegree; ++j)
		{
			exponent = mDegree - j;
			matrix[i][j] = qPow(i, exponent); // Fill the first half of the matrix with the Hermite polynomials constructed from the x-values
			matrix[i + newSize][j + mDegree] = exponent * qPow(i, exponent - 1); // Fill the second half of the matrix with the derivitaves
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
		//	for(j = 0; j < count; ++j) value += coefficients[j] * qPow(x, mDegree - j);

			for(j = 0; j < mDegree; ++j)
						{
							exponent = mDegree - j;
							value += coefficients[j] * qPow(x, exponent);
							value += exponent * coefficients[j+mDegree] * qPow(x, exponent - 1);
							//cout<<coefficients[j]<<"\t"<<coefficients[j]<<"\t"<<value<<endl;
						}
			//cout<<"********************: "<<endl;
			predictedSamples[i] = value;
		}
	}
	else
	{
		for(i = 0; i < predictionCount; ++i) predictedSamples[i] = 0;
	}
}
