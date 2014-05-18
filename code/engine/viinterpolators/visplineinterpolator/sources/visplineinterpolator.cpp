#include <visplineinterpolator.h>
#include <visystemsolver.h>

#define DEFAULT_DEGREE 3

ViSplineInterpolator::ViSplineInterpolator()
	: ViDegreeInterpolator(DEFAULT_DEGREE)
{
}

ViSplineInterpolator::ViSplineInterpolator(const int &degree)
	: ViDegreeInterpolator(degree)
{
}

void ViSplineInterpolator::setParameters(const qreal &param1, const qreal &param2)
{
	setWindowSize(param1);
	setDegree(param2);
}

bool ViSplineInterpolator::interpolate(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize)
{
	static int i, j, index, xIndex, x1, x2, end, end2, indexes, parameterCount, size, derivative, advance, advance2, row, front, exponent;

	end = leftSize + rightSize;
	indexes = end - 1;
	parameterCount = mDegree + 1;
	size = parameterCount * indexes;

	ViVector vector(size);
	ViMatrix matrix(size, size);

	vector[0] = (0 < leftSize) ? leftSamples[0] : rightSamples[leftSize];
	for(i = 1; i < indexes; ++i)
	{
		index = i * 2;
		vector[index] = vector[index - 1] = (i < leftSize) ? leftSamples[i] : rightSamples[i - leftSize];
	}
	vector[(indexes * 2) - 1] = (indexes < leftSize) ? leftSamples[indexes] : rightSamples[indexes - leftSize];
	// The rest of the vector is 0

	// Add the functions for each spline between two points
	for(i = 0; i < indexes; ++i)
	{
		index = i * 2;
		ViVector &row1 = matrix[index];
		ViVector &row2 = matrix[index + 1];

		x1 = (i < leftSize) ? i : i + outputSize;
		xIndex = i + 1;
		x2 = (xIndex < leftSize) ? xIndex : xIndex + outputSize;

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
			x1 = (xIndex < leftSize) ? xIndex : xIndex + outputSize;

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
		row = size - end + i;
		advance = i * parameterCount;
		advance2 = size - parameterCount;

		x1 = (0 < leftSize) ? 0 : outputSize;
		x2 = (indexes < leftSize) ? indexes : outputSize;

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
		index = (leftSize - 1) * parameterCount;
		for(i = 0; i < outputSize; ++i)
		{
			x1 = leftSize + i;
			value = 0;
			for(j = 0; j < parameterCount; ++j)
			{
				value += coefficients[index + j] * qPow(x1, mDegree - j);
			}
			outputSamples[i] = value;
		}
		return true;
	}
	return false;
}

qreal ViSplineInterpolator::calculateMultiplier(const int &derivative, const int &parameterNumber)
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

ViSplineInterpolator* ViSplineInterpolator::clone()
{
	return new ViSplineInterpolator(*this);
}

#ifdef __cplusplus
extern "C"
{
#endif

ViSplineInterpolator* create()
{
	return new ViSplineInterpolator();
}

#ifdef __cplusplus
}
#endif
