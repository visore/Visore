#include <viautoregresser.h>
#include <vilogger.h>

ViAutoRegresser::ViAutoRegresser()
{
	mModelOrder = NULL;
}

ViAutoRegresser::ViAutoRegresser(const ViAutoRegresser &other)
{
	if(other.mModelOrder != NULL)
	{
		mModelOrder = other.mModelOrder->clone();
	}
}

ViAutoRegresser::~ViAutoRegresser()
{
	clear();
}

void ViAutoRegresser::clear()
{
	if(mModelOrder != NULL)
	{
		delete mModelOrder;
		mModelOrder = NULL;
	}
	mCoefficients.clear();
	mCacheMatrix.clear();
}

void ViAutoRegresser::setModelOrder(ViModelOrder *modelOrder)
{
	clear();
	mModelOrder = modelOrder;
}

ViModelOrder* ViAutoRegresser::modelOrder()
{
	return mModelOrder;
}

ViVector& ViAutoRegresser::coefficients()
{
	return mCoefficients;
}

const ViVector& ViAutoRegresser::coefficients() const
{
	return mCoefficients;
}

bool ViAutoRegresser::calculate(const ViSampleChunk &samples)
{
	int sampleCount = samples.size();
	mModelOrder->setSampleCount(sampleCount);

	if(mModelOrder == NULL) return false;
	int order = mModelOrder->order();

	// http://mathworld.wolfram.com/LeastSquaresFittingPolynomial.html

	ViVector vector(sampleCount);

	if(mCacheMatrix.rows() != sampleCount || mCacheMatrix.columns() != order + 1)
	{
		// Only create the matrix if it wasn't previously created
		mCacheMatrix = ViMatrix(sampleCount, order + 1);
		for(int i = 0; i < sampleCount; ++i)
		{
			vector[i] = samples[i];
			mCacheMatrix[i][0] = 1;
			for(int j = 1; j <= order; ++j)
			{
				mCacheMatrix[i][j] = qPow(i, j);
			}
		}
	}
	else
	{
		for(int i = 0; i < sampleCount; ++i)
		{
			vector[i] = samples[i];
		}
	}

	ViMatrix transpose = mCacheMatrix.transpose();
	ViMatrix inverted;
	if(!transpose.scalarMultiply(mCacheMatrix).invert(inverted)) return false;
	mCoefficients = inverted.scalarMultiply(transpose) * vector;
	return true;
}
