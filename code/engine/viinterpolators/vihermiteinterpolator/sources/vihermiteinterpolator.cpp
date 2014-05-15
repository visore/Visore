#include <vihermiteinterpolator.h>
#include <visystemsolver.h>

#define DEFAULT_DEGREE 3

ViHermiteInterpolator::ViHermiteInterpolator()
	: ViDegreeInterpolator(DEFAULT_DEGREE)
{
}

ViHermiteInterpolator::ViHermiteInterpolator(const int &degree)
	: ViDegreeInterpolator(degree)
{

}

void ViHermiteInterpolator::setParameters(const qreal &param1, const qreal &param2)
{
	setWindowSize(param1);
	setDegree(param2);
}

bool ViHermiteInterpolator::interpolateSamples(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize)
{
	static int i, j, x, newLeftSize, sampleCount, exponent;

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
