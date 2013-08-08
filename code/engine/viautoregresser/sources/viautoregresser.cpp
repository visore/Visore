#include <viautoregresser.h>
#include <vilogger.h>

#define DEFAULT_ORDER 8

ViAutoRegresser::ViAutoRegresser()
{
	clear();
}

ViAutoRegresser::ViAutoRegresser(const ViAutoRegresser &other)
{
	mOrder = other.mOrder;
	mCacheMatrix = other.mCacheMatrix;
	mCacheCalculatedMatrix = other.mCacheCalculatedMatrix;
	mCoefficients = other.mCoefficients;
	mRss = other.mRss;
}

ViAutoRegresser::~ViAutoRegresser()
{
	clear();
}

void ViAutoRegresser::clear()
{
	mOrder = DEFAULT_ORDER;
	mCoefficients.clear();
	mCacheMatrix.clear();
	mCacheCalculatedMatrix.clear();
	mRss = 0;
}

void ViAutoRegresser::setOrder(const int &order)
{
	mOrder = order;
}

int ViAutoRegresser::order() const
{
	return mOrder;
}

ViVector& ViAutoRegresser::coefficients()
{
	return mCoefficients;
}

const ViVector& ViAutoRegresser::coefficients() const
{
	return mCoefficients;
}

qreal ViAutoRegresser::rss() const
{
	return mRss;
}

bool ViAutoRegresser::calculate(const ViSampleChunk &samples)
{
	if(mOrder < 3) return false;

	// http://mathworld.wolfram.com/LeastSquaresFittingPolynomial.html

	int i, j, sampleCount = samples.size();
	ViVector vector(sampleCount);

	// Only create a new matrix if the previously cached one isn't the same
	if(mCacheMatrix.rows() != sampleCount || mCacheMatrix.columns() != mOrder + 1)
	{
		mCacheMatrix = ViMatrix(sampleCount, mOrder + 1);
		for(i = 0; i < sampleCount; ++i)
		{
			vector[i] = samples[i];
			mCacheMatrix[i][0] = 1;
			for(j = 1; j <= mOrder; ++j)
			{
				mCacheMatrix[i][j] = qPow(i, j);
			}
		}
		ViMatrix transpose = mCacheMatrix.transpose();
		ViMatrix inverted;
		if(!transpose.scalarMultiply(mCacheMatrix).invert(inverted)) return false;
		mCacheCalculatedMatrix = inverted.scalarMultiply(transpose);
	}
	else
	{
		for(i = 0; i < sampleCount; ++i)
		{
			vector[i] = samples[i];
		}
	}

	mCoefficients = mCacheCalculatedMatrix * vector;

	// Calculate the RSS
	qreal real;
	mRss = 0.0;
	for(i = 0; i < sampleCount; ++i)
	{
		real = 0;
		for(j = 0; j <= mOrder; ++j)
		{
			real += mCoefficients[j] * mCacheMatrix[i][j];
		}
		mRss += qPow(vector[i] - real, 2);
	}

	return true;
}
