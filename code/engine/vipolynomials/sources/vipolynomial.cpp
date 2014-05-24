#include <vipolynomial.h>
#include <vilogger.h>

#define MAX_VECTOR_SIZE 16384

ViPolynomial::ViPolynomial()
{
	mDegree = 0;
	mDerivatives = 0;
}

ViPolynomial::ViPolynomial(const ViPolynomial &other)
{
	mDegree = other.mDegree;
	mCoefficients = other.mCoefficients;
	mDerivatives = other.mDerivatives;
	mCache = other.mCache;
}

ViPolynomial::~ViPolynomial()
{
}

void ViPolynomial::setDerivatives(const int &derivatives)
{
	mDerivatives = derivatives;
	createCache();
}

void ViPolynomial::setDegree(const int &degree)
{
	mDegree = degree;
	mCoefficients = maxCoefficients(mDegree);
	createCache();
}

void ViPolynomial::createCache()
{
	qreal i; // Must be qreal, since we devide it
	for(int j = 0; j <= mDerivatives; ++j)
	{
		if(j >= mCache.size()) mCache.append(QVector<QVector<ViVector>>());
		QVector<QVector<ViVector>> &currentCache = mCache[j];
		if(mDegree >= currentCache.size()) currentCache.resize(mDegree + 1);
		QVector<ViVector> &current = currentCache[mDegree];
		if(current.isEmpty())
		{
			current.resize(MAX_VECTOR_SIZE + 1);

			// Devide x (the time-lags). Since they are being squared, a large x can casue a value overflow. Needs to be a double, since we devide.
			// This division is extremly important for Fourier polynomials. Since me multiply with PI in Fourier, if i is not scaled here, all the sin terms will evaluate to 0 (sin(xPI): sin(PI) = 0, sin(2PI) = 0, sin(3PI) = 0, ...)
			QVector<qreal> x(MAX_VECTOR_SIZE + 1);
			for(i = 0; i <= MAX_VECTOR_SIZE; ++i) x[i] = i / MAX_VECTOR_SIZE;

			for(i = 0; i <= MAX_VECTOR_SIZE; ++i)
			{
				ViVector row(mCoefficients);
				construct(mDegree, x[i], x, row, j);
				current[i] = row;
			}
		}
	}
}

const int& ViPolynomial::degree() const
{
	return mDegree;
}

const int& ViPolynomial::coefficientCount() const
{
	return mCoefficients;
}

void ViPolynomial::get(const int &degree, const int &x, ViVector &row, const int &derivative)
{
	row = mCache[derivative][degree][x];
}

const ViVector* ViPolynomial::get(const int &degree, const int &derivative)
{
	return mCache[derivative][degree].data();
}

const ViVector* ViPolynomial::getOffset(const int &degree, const int &x, const int &derivative)
{
	return mCache[derivative][degree].data() + x;
}
