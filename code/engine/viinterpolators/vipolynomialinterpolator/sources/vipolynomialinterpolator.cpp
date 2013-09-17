#include <vipolynomialinterpolator.h>
#include <visystemsolver.h>

#define DEFAULT_DEGREE 3
#define MAXIMUM_SAMPLES -1

ViPolynomialInterpolator::ViPolynomialInterpolator()
	: ViDegreeInterpolator(MAXIMUM_SAMPLES, DEFAULT_DEGREE)
{
}

bool ViPolynomialInterpolator::interpolateSamples(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize)
{
	static int i, j, x, index, sampleCount;

	sampleCount = leftSize + rightSize;
	ViMatrix matrix(sampleCount, mDegree + 1);
	ViVector vector(sampleCount);

	for(i = 0; i < leftSize; ++i)
	{
		vector[i] = leftSamples[i];
		matrix[i][0] = 1;
		for(j = 1; j <= mDegree; ++j)
		{
			matrix[i][j] = qPow(i, j);
		}
	}
	for(i = 0; i < rightSize; ++i)
	{
		index = i + leftSize;
		x = index + outputSize;
		vector[index] = rightSamples[i];
		matrix[index][0] = 1;
		for(j = 1; j <= mDegree; ++j)
		{
			matrix[index][j] = qPow(x, j);
		}
	}

	static ViVector coefficients;
	if(ViSystemSolver::solve(matrix, vector, coefficients))
	{
		static qreal value;
		static int count;
		count = coefficients.size();
		for(i = 0; i < outputSize; ++i)
		{
			x = leftSize + i;
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
