#include <vipolynomialinterpolator.h>
#include <vimodelsolver.h>

#include<vilogger.h>

#define DEFAULT_DEGREE 3
#define MAXIMUM_SIZE 16 // Maximum size of matrix. If too big, computation is very slow

ViPolynomialInterpolator::ViPolynomialInterpolator()
	: ViInterpolator()
{
	mDegree = DEFAULT_DEGREE;
}

void ViPolynomialInterpolator::setDegree(const int &degree)
{
	mDegree = degree;
}

int ViPolynomialInterpolator::degree()
{
	return mDegree;
}

bool ViPolynomialInterpolator::interpolateSamples(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize)
{
	if(mDegree < 1) return false;

	int newLeftSize, newRightSize;
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

	int i, j, x, index, sampleCount = newLeftSize + newRightSize;
	ViMatrix matrix(sampleCount, mDegree + 1);
	ViVector coefficients, samples(sampleCount);
	for(i = 0; i < newLeftSize; ++i)
	{
		samples[i] = leftSamples[i];
		matrix[i][0] = 1;
		for(j = 1; j <= mDegree; ++j)
		{
			matrix[i][j] = qPow(i, j);
		}
	}
	for(i = 0; i < newRightSize; ++i)
	{
		index = i + newLeftSize;
		x = index + outputSize;
		samples[index] = rightSamples[i];
		matrix[index][0] = 1;
		for(j = 1; j <= mDegree; ++j)
		{
			matrix[index][j] = qPow(x, j);
		}
	}

	if(ViModelSolver::estimate(mDegree, matrix, samples, coefficients))
	{
		qreal value;
		int count = coefficients.size();
		for(i = 0; i < outputSize; ++i)
		{
			x = newLeftSize + i;
			value = 0;
			for(j = 0; j < count; ++j)
			{
				value += coefficients[count - j - 1] * qPow(x, mDegree - j);
			}
			outputSamples[i] = value;
		}
		return true;
	}
	return false;
}

ViPolynomialInterpolator* ViPolynomialInterpolator::clone()
{
	return new ViPolynomialInterpolator(*this);
}

#ifdef __cplusplus
extern "C"
{
#endif

ViPolynomialInterpolator* create()
{
	return new ViPolynomialInterpolator();
}

#ifdef __cplusplus
}
#endif
