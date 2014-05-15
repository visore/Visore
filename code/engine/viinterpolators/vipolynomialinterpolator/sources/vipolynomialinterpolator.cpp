#include <vipolynomialinterpolator.h>
#include <visystemsolver.h>
#include<vilogger.h>

#define DEFAULT_DEGREE 3

ViPolynomialInterpolator::ViPolynomialInterpolator()
	: ViDegreeInterpolator(DEFAULT_DEGREE)
{
}

ViPolynomialInterpolator::ViPolynomialInterpolator(const int &degree)
	: ViDegreeInterpolator(degree)
{
}

void ViPolynomialInterpolator::setParameters(const qreal &param1, const qreal &param2)
{
	setWindowSize(param1);
	setDegree(param2);
}

bool ViPolynomialInterpolator::interpolateSamples(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize)
{

	/*static int i, j, x, index, sampleCount;

		sampleCount = leftSize;
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

		static ViVector coefficients;
		if(ViSystemSolver::solve(matrix, vector, coefficients))
		{
			//LOG(coefficients.toString());
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
		else
		{
			for(i = 0; i < outputSize; ++i)
			{
				outputSamples[i] = 0;
			}
		}
		return false;*/

	/*if(leftSize + rightSize <= mDegree) // Not enough samples
	{
		for(i = 0; i < outputSize; ++i)
		{
			outputSamples[i] = -99;
		}
		return false;
	}*/

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

bool ViPolynomialInterpolator::solveEquations(double **matrix, double *coefficients, const int &degree)
{
	static int i, j, k, maxi, end;
	static qreal vswap, max, h, pivot, q;
	static qreal *mswap, *hvec;

	end = degree - 1;
	for(i = 0; i < end; ++i)
	{
		max = fabs(matrix[i][i]);
		maxi = i;
		for(j = i + 1; j < degree; ++j)
		{
			if((h = fabs(matrix[j][i])) > max)
			{
				max = h;
				maxi = j;
			}
		}
		if(maxi != i)
		{
			mswap = matrix[i];
			matrix[i] = matrix[maxi];
			matrix[maxi] = mswap;
			vswap = coefficients[i];
			coefficients[i] = coefficients[maxi];
			coefficients[maxi] = vswap;
		}

		hvec = matrix[i];
		pivot = hvec[i];
		if(fabs(pivot) == 0.0) return false;

		for(j = i + 1; j < degree; ++j)
		{
			q = - matrix[j][i] / pivot;
			matrix[j][i] = 0.0;
			for(k = i + 1; k < degree; ++k)
			{
				matrix[j][k] += q * hvec[k];
			}
			coefficients[j] += (q * coefficients[i]);
		}
	}

	coefficients[end] /= matrix[end][end];
	for(i = degree - 2; i >= 0; --i)
	{
		hvec = matrix[i];
		for(j = end; j > i; --j)
		{
			coefficients[i] -= (hvec[j] * coefficients[j]);
		}
		coefficients[i] /= hvec[i];
	}
	return true;
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
