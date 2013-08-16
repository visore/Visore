#include <vimodelsolver.h>
#include <vilogger.h>

#define DEFAULT_ORDER 8

ViModelSolver::ViModelSolver()
{
	clear();
}

ViModelSolver::ViModelSolver(const ViModelSolver &other)
{
	mOrder = other.mOrder;
	mCacheMatrix = other.mCacheMatrix;
	mCacheCalculatedMatrix = other.mCacheCalculatedMatrix;
	mCoefficients = other.mCoefficients;
	mRss = other.mRss;
}

ViModelSolver::~ViModelSolver()
{
	clear();
}

void ViModelSolver::clear()
{
	mOrder = DEFAULT_ORDER;
	mCoefficients.clear();
	mCacheMatrix.clear();
	mCacheCalculatedMatrix.clear();
	mRss = 0;
}

void ViModelSolver::setOrder(const int &order)
{
	mOrder = order;
}

int ViModelSolver::order() const
{
	return mOrder;
}

ViVector& ViModelSolver::coefficients()
{
	return mCoefficients;
}

const ViVector& ViModelSolver::coefficients() const
{
	return mCoefficients;
}

qreal ViModelSolver::rss() const
{
	return mRss;
}

bool ViModelSolver::estimate(const ViVector &samples)
{
	return estimateCoefficients(mOrder, samples, mCacheMatrix, mCacheCalculatedMatrix, mCoefficients, mRss);
}

bool ViModelSolver::estimate(const ViSampleChunk &samples)
{
	ViVector vector(samples.size(), samples.data(), true);
	return estimateCoefficients(mOrder, vector, mCacheMatrix, mCacheCalculatedMatrix, mCoefficients, mRss);
}

ViVector ViModelSolver::estimate(const int &order, const ViVector &samples)
{
	ViVector coefficients;
	estimateCoefficients(order, samples, coefficients);
	return coefficients;
}

ViVector ViModelSolver::estimate(const int &order, const ViSampleChunk &samples)
{
	ViVector vector(samples.size(), samples.data(), true);;
	return estimate(order, vector);
}

bool ViModelSolver::estimate(const int &order, const ViVector &samples, ViVector &coefficients, qreal &rss)
{
	return estimateCoefficients(order, samples, coefficients);
}

bool ViModelSolver::estimate(const int &order, const ViSampleChunk &samples, ViVector &coefficients)
{
	ViVector vector(samples.size(), samples.data(), true);
	return estimate(order, vector, coefficients);
}

bool ViModelSolver::estimate(const int &order, const ViVector &samples, ViVector &coefficients)
{
	return estimateCoefficients(order, samples, coefficients);
}

bool ViModelSolver::estimate(const int &order, const ViSampleChunk &samples, ViVector &coefficients, qreal &rss)
{
	ViVector vector(samples.size(), samples.data(), true);
	return estimate(order, vector, coefficients, rss);
}

ViVector ViModelSolver::estimate(const int &order, const ViMatrix &matrix, const ViVector &vector)
{
	ViVector coefficients;
	estimateCoefficients(order, vector, matrix, coefficients);
	return coefficients;
}

bool ViModelSolver::estimate(const int &order, const ViMatrix &matrix, const ViVector &vector, ViVector &coefficients)
{
	return estimateCoefficients(order, vector, matrix, coefficients);
}

bool ViModelSolver::estimateCoefficients(const int &order, const ViVector &samples, ViMatrix &cached, ViMatrix &calculatedCached, ViVector &coefficients, qreal &rss)
{
	rss = 0.0;
	if(estimateCoefficients(order, samples, cached, calculatedCached, coefficients))
	{
		// Calculate the RSS
		qreal real;
		int i, j, sampleCount = samples.size();
		for(i = 0; i < sampleCount; ++i)
		{
			real = 0;
			for(j = 0; j <= order; ++j)
			{
				real += coefficients[j] * cached[i][j];
			}
			rss += qPow(samples[i] - real, 2);
		}
		return true;
	}
	return false;
}

bool ViModelSolver::estimateCoefficients(const int &order, const ViVector &samples, ViMatrix &cached, ViMatrix &calculatedCached, ViVector &coefficients)
{
	if(order < 1)
	{
		STATICLOG("The minimum model order is 1.", QtCriticalMsg, "ViModelSolver");
		return false;
	}

	// http://mathworld.wolfram.com/LeastSquaresFittingPolynomial.html

	int i, j, sampleCount = samples.size();
	if(order >= sampleCount)
	{
		STATICLOG("The model can not be greater or equal to the sample count.", QtCriticalMsg, "ViModelSolver");
		return false;
	}

	// Only create a new matrix if the previously cached one isn't the same
	bool calculate = false;
	if(cached.rows() != sampleCount || cached.columns() != order + 1)
	{
		cached = ViMatrix(sampleCount, order + 1);
		for(i = 0; i < sampleCount; ++i)
		{
			cached[i][0] = 1;
			for(j = 1; j <= order; ++j)
			{
				cached[i][j] = qPow(i, j);
			}
		}
		calculate = true;
	}
	if(calculate || !calculatedCached.isValid())
	{
		ViMatrix inverted, transpose = cached.transpose();
		if(!transpose.scalarMultiply(cached).invert(inverted))
		{
			STATICLOG("The matrix cannot be inverted.", QtCriticalMsg, "ViModelSolver");
			return false;
		}
		calculatedCached = inverted.scalarMultiply(transpose);
	}

	coefficients = calculatedCached * samples;

	return true;
}

bool ViModelSolver::estimateCoefficients(const int &order, const ViVector &samples, ViVector &coefficients)
{
	ViMatrix cached, calculatedCached;
	return estimateCoefficients(order, samples, cached, calculatedCached, coefficients);
}

bool ViModelSolver::estimateCoefficients(const int &order, const ViVector &samples, const ViMatrix &cached, ViVector &coefficients)
{
	if(order < 1)
	{
		STATICLOG("The minimum model order is 1.", QtCriticalMsg, "ViModelSolver");
		return false;
	}

	int sampleCount = samples.size();
	if(order >= sampleCount)
	{
		STATICLOG("The model can not be greater or equal to the sample count.", QtCriticalMsg, "ViModelSolver");
		return false;
	}

	ViMatrix inverted, transpose = cached.transpose();
	if(!transpose.scalarMultiply(cached).invert(inverted))
	{
		STATICLOG("The matrix cannot be inverted.", QtCriticalMsg, "ViModelSolver");
		return false;
	}
	coefficients = inverted.scalarMultiply(transpose) * samples;
	return true;
}
