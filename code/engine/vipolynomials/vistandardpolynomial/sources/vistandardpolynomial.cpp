#include <vistandardpolynomial.h>
#include<vilogger.h>

ViStandardPolynomial::ViStandardPolynomial()
	: ViPolynomial()
{
}

ViStandardPolynomial::ViStandardPolynomial(const ViStandardPolynomial &other)
	: ViPolynomial(other)
{
}

ViStandardPolynomial::~ViStandardPolynomial()
{
}

void ViStandardPolynomial::construct(const int &degree, const qreal &x, const QVector<qreal> &allX, ViVector &row, const int &derivative)
{
	if(degree == 0 || derivative > degree)
	{
		for(mI = 0; mI <= degree; ++mI) row[mI] = 0;
	}
	else if(derivative == 0)
	{
		row[0] = 1;
		row[1] = x;
		for(mI = 2; mI <= degree; ++mI) row[mI] = qPow(x, mI);
	}
	else
	{
		for(mI = 0; mI < derivative; ++mI) row[mI] = 0;
		row[derivative] = 1;
		for(mI = derivative + 1; mI <= degree; ++mI) row[mI] = mI * qPow(x, mI - 1);
	}
}

void ViStandardPolynomial::solve(const int &degree, const int &offset, const ViVector &coefficients, qreal &result, const int &derivative)
{
	if(degree == 0)
	{
		result = 0;
	}
	else if(derivative == 0)
	{
		const ViVector &vector = getOffset(degree, offset)[0];
		result = coefficients[0];
		for(mI = 1; mI <= degree; ++mI) result += coefficients[mI] * vector[mI];
	}
	else
	{
		const ViVector &vector = getOffset(degree, offset, derivative)[0];
		result = coefficients[derivative];
		for(mI = derivative + 1; mI <= degree; ++mI) result += coefficients[mI] * vector[mI];
	}
}

int ViStandardPolynomial::maxCoefficients(const int &degree) const
{
	return degree + 1;
}

ViStandardPolynomial* ViStandardPolynomial::clone() const
{
	return new ViStandardPolynomial(*this);
}
