#include <vinewtonpolynomial.h>
#include<vilogger.h>

ViNewtonPolynomial::ViNewtonPolynomial()
	: ViPolynomial()
{
}

ViNewtonPolynomial::ViNewtonPolynomial(const ViNewtonPolynomial &other)
	: ViPolynomial(other)
{
}

ViNewtonPolynomial::~ViNewtonPolynomial()
{
}

void ViNewtonPolynomial::construct(const int &degree, const qreal &x, const QVector<qreal> &allX, ViVector &row, const int &derivative)
{
	if(degree == 0)
	{
		for(mI = 0; mI <= degree; ++mI) row[mI] = 0;
	}
	else if(derivative == 0)
	{
		row[0] = 1;
		for(mI = 1; mI <= degree; ++mI)
		{
			mValue1 = 1;
			for(mJ = 0; mJ < mI; ++mJ) mValue1 *= x - allX[mJ];
			row[mI] = mValue1;
		}
	}
	else
	{

	}
}

void ViNewtonPolynomial::solve(const int &degree, const int &offset, const ViVector &coefficients, qreal &result, const int &derivative)
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

	}
}

int ViNewtonPolynomial::maxCoefficients(const int &degree) const
{
	return degree + 1;
}

ViNewtonPolynomial* ViNewtonPolynomial::clone() const
{
	return new ViNewtonPolynomial(*this);
}
