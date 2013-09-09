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

	static int i, j, x, newLeftSize, newRightSize, sampleCount;
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
	if(leftSize < 2 || rightSize < 2) return false;

	sampleCount = newLeftSize + newRightSize;
	ViVector coefficients, vector(sampleCount * 2);
	ViMatrix matrix(sampleCount * 2, mDegree + 1);

	// Fill the first half of the vector with the y-values
	for(i = 0; i < sampleCount; ++i)
	{
		vector[i] = (i < newLeftSize) ? leftSamples[i] : rightSamples[i - newLeftSize];
	}

	// Fill the second half of the vector with the outgoing slopes
	for(i = 0; i < sampleCount; ++i)
	{
		if(i < newLeftSize)
		{
			vector[i + sampleCount] = (leftSamples[i] - leftSamples[i - 1]);
		}
		else
		{
			vector[i + sampleCount] = (rightSamples[i - newLeftSize + 1] - rightSamples[i - newLeftSize]);
		}
	}

	// Fill the first half of the matrix with the Hermite polynomials constructed from the x-values
	for(i = 0; i < sampleCount; ++i)
	{
		for(j = 0; j < mDegree; ++j)
		{
			x = (i < newLeftSize) ? i : i + outputSize;
			matrix[i][j] = qPow(x, mDegree - j);
		}
		matrix[i][mDegree] = 1;
	}

	// Fill the second half of the matrix with the derivitaves
	for(i = 0; i < sampleCount; ++i)
	{
		for(j = 0; j < mDegree; ++j)
		{
			x = (i < newLeftSize) ? i : i + outputSize;
			matrix[i + sampleCount][j] = (mDegree - j) * qPow(x, mDegree - j - 1);
		}
	}

	if(ViModelSolver::estimate(mDegree, matrix, vector, coefficients))
	{
		/*cout<<newLeftSize<<" "<<newRightSize<<endl;
		cout<<vector.toString().toLatin1().data()<<endl;
		cout<<"**********************************"<<endl<<matrix.toString().toLatin1().data()<<endl;
		cout<<"**********************************"<<endl<<coefficients.toString().toLatin1().data()<<endl;
*/
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
			//outputSamples[i] = value;
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
