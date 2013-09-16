#include <vihermiteinterpolator.h>
#include <vimodelsolver.h>

#include<vilogger.h>

#define DEFAULT_DEGREE 3
#define MAXIMUM_SIZE 16 // Maximum size of matrix. If too big, computation is very slow

ViHermiteInterpolator::ViHermiteInterpolator()
	: ViInterpolator()
{
	mDegree = DEFAULT_DEGREE;
}

ViHermiteInterpolator::ViHermiteInterpolator(const int &degree)
	: ViInterpolator()
{
	mDegree = degree;
}

void ViHermiteInterpolator::setDegree(const int &degree)
{
	mDegree = degree;
}

int ViHermiteInterpolator::degree()
{
	return mDegree;
}

bool ViHermiteInterpolator::interpolateSamples(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize)
{
	if(mDegree < 1) return false;

	static int i, j, x, newLeftSize, newRightSize, sampleCount, exponent;
	if(leftSize > MAXIMUM_SIZE)
	{
		leftSamples += (leftSize - MAXIMUM_SIZE);
		newLeftSize = MAXIMUM_SIZE;
	}
	else
	{
		newLeftSize = leftSize;
	}

	if(rightSize > MAXIMUM_SIZE)
	{
		newRightSize = MAXIMUM_SIZE;
	}
	else
	{
		newRightSize = rightSize;
	}

	// We need outgoing slopes, so we can't use the first or last point (used to calculate the second and second-last slope)
	--newLeftSize;
	--newRightSize;
	++leftSamples;
	if(newLeftSize < 2 || newRightSize < 2) return false;

	sampleCount = newLeftSize + newRightSize;
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
	if(ViModelSolver::estimate(mDegree, matrix, vector, coefficients))
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
	return false;
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
