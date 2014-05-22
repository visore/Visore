#include <visplinepredictor.h>
#include <visystemsolver.h>
#include <vilogger.h>

#define DEFAULT_DEGREE 2

ViSplinePredictor::ViSplinePredictor()
	: ViPredictor()
{
	setDegree(DEFAULT_DEGREE);

	addParameterName("Window Size");
	addParameterName("Degree");
}

ViSplinePredictor::ViSplinePredictor(const ViSplinePredictor &other)
	: ViPredictor(other)
{
	mDegree = other.mDegree;
}

ViSplinePredictor::~ViSplinePredictor()
{
}

void ViSplinePredictor::setDegree(const int &degree)
{
	mDegree = degree;
}

void ViSplinePredictor::setParameter(const int &number, const qreal &value)
{
	if(number == 0) setWindowSize(value);
	else if(number == 1) setDegree(value);
	else
	{
		LOG("Invalid parameter for this predictor.", QtCriticalMsg);
		exit(-1);
	}
}

void ViSplinePredictor::predict(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount)
{
	static int i, j, index, xIndex, x1, x2, end, end2, indexes, parameterCount, sizeT, derivative, advance, advance2, row, front, exponent;

	end = size;
	indexes = end - 1;
	parameterCount = mDegree + 1;
	sizeT = parameterCount * indexes;

	ViVector vector(sizeT);
	ViMatrix matrix(sizeT, sizeT);

	vector[0] = samples[0];
	for(i = 1; i < indexes; ++i)
	{
		index = i * 2;
		vector[index] = vector[index - 1] = samples[i];
	}
	vector[(indexes * 2) - 1] = samples[indexes];
	// The rest of the vector is 0

	// Add the functions for each spline between two points
	for(i = 0; i < indexes; ++i)
	{
		index = i * 2;
		ViVector &row1 = matrix[index];
		ViVector &row2 = matrix[index + 1];

		x1 = i;
		xIndex = i + 1;
		x2 = xIndex;

		for(j = 0; j < parameterCount; ++j)
		{
			index = (i * parameterCount) + j;
			row1[index] = qPow(x1, mDegree - j);
			row2[index] = qPow(x2, mDegree - j);
		}
	}

	// Add the slope (first derivatives) of all points with a spline at each side
	for(derivative = 0; derivative < mDegree - 1; ++derivative)
	{
		end = indexes - 1; // (n - 2) where n is the number of data points
		for(i = 0; i < end; ++i)
		{
			row = (indexes * 2) + i + (derivative * end);
			advance = i * parameterCount;
			advance2 = advance + parameterCount;

			xIndex = i + 1;
			x1 =  xIndex;

			end2 = parameterCount - 1 - derivative;
			for(j = 0; j < end2; ++j)
			{
				front = calculateMultiplier(derivative + 1, j);
				exponent = mDegree - derivative - j - 1;
				matrix[row][advance + j] = front * qPow(x1, exponent);
				matrix[row][advance2 + j] = -front * qPow(x1, exponent);
			}
		}
	}

	// Add the second derivatives. Second derivatives should vanish at the end points (aka towards 0)
	end = mDegree - 1;
	for(i = 0; i < end - 1; ++i)
	{
		row = sizeT - end + i;
		advance = i * parameterCount;
		advance2 = sizeT - parameterCount;

		x1 =  0 ;
		x2 =  indexes;

		end2 = parameterCount - 2;
		for(j = 0; j < end2; ++j)
		{
			front = calculateMultiplier(2, j);
			exponent = mDegree - 2 - j;
			matrix[row][advance + j] = front * qPow(x1, exponent);
			matrix[row + 1][advance2 + j] = front * qPow(x2, exponent);
		}
	}

	static ViVector coefficients;
	if(ViSystemSolver::solve(matrix, vector, coefficients))
	{
		qreal value;
		index = (size - 1) * parameterCount;
		for(i = 0; i < predictionCount; ++i)
		{
			x1 = size + i;
			value = 0;
			for(j = 0; j < parameterCount; ++j)
			{
				value += coefficients[index + j] * qPow(x1, mDegree - j);
			}
			predictedSamples[i] = value;
		}
	}
	else
	{
		for(i = 0; i < predictionCount; ++i) predictedSamples[i] = 0;
	}
}

qreal ViSplinePredictor::calculateMultiplier(const int &derivative, const int &parameterNumber)
{
	static qreal result;
	static int multiply, i;

	result = 1;
	multiply = mDegree - parameterNumber;
	for(i = 0; i < derivative; ++i)
	{
		result *= multiply - i;
	}
	return result;
}
