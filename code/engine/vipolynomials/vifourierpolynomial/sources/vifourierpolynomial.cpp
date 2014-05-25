#include <vifourierpolynomial.h>
#include<vilogger.h>

ViFourierPolynomial::ViFourierPolynomial()
	: ViPolynomial()
{
}

ViFourierPolynomial::ViFourierPolynomial(const ViFourierPolynomial &other)
	: ViPolynomial(other)
{
}

ViFourierPolynomial::~ViFourierPolynomial()
{
}

void ViFourierPolynomial::construct(const int &degree, const qreal &x, const QVector<qreal> &allX, ViVector &row, const int &derivative)
{
	if(degree == 0)
	{
		for(mI = 0; mI <= degree; ++mI) row[mI] = 0;
	}
	else if(derivative == 0)
	{		
		mValue1 = M_PI * x; // Multiple of Pi, since we work with sin and cos
		row[0] = 0.5;
		for(mI = 1; mI <= degree; ++mI)
		{
			mValue2 = mValue1 * mI;
			row[mI] = qCos(mValue2);
			row[mI + degree] = qSin(mValue2);
		}
	}
	else
	{
		mValue1 = M_PI * x; // Multiple of Pi, since we work with sin and cos
		row[0] = 0;
		if(derivative % 4 == 0) // 4th, 8th, 12th, ... derivatives
		{
			for(mI = 1; mI <= degree; ++mI)
			{
				mValue2 = mValue1 * mI;
				mValue3 = qPow(mI, derivative);
				row[mI] = mValue3 * qCos(mValue2);
				row[mI + degree] = mValue3 * qSin(mValue2);
			}
		}
		else if(derivative % 4 == 1) // 1st, 5th, 9th, ... derivatives
		{
			for(mI = 1; mI <= degree; ++mI)
			{
				mValue2 = mValue1 * mI;
				mValue3 = qPow(mI, derivative);
				row[mI] = mValue3 * qCos(mValue2);
				row[mI + degree] = -mValue3 * qSin(mValue2);
			}
		}
		else if(derivative % 4 == 2) // 2nd, 6th, 10th, ... derivatives
		{
			for(mI = 1; mI <= degree; ++mI)
			{
				mValue2 = mValue1 * mI;
				mValue3 = -qPow(mI, derivative);
				row[mI] = mValue3 * qCos(mValue2);
				row[mI + degree] = mValue3 * qSin(mValue2);
			}
		}
		else if(derivative % 4 == 3) // 3rd, 7th, 1th, ... derivatives
		{
			for(mI = 1; mI <= degree; ++mI)
			{
				mValue2 = mValue1 * mI;
				mValue3 = qPow(mI, derivative);
				row[mI] = -mValue3 * qCos(mValue2);
				row[mI + degree] = mValue3 * qSin(mValue2);
			}
		}
	}
}

void ViFourierPolynomial::solve(const int &degree, const int &offset, const ViVector &coefficients, qreal &result, const int &derivative)
{
	if(derivative == 0 && degree != 0)
	{
		const ViVector &vector = getOffset(degree, offset)[0];
		result = coefficients[0] / 2.0;
		for(mI = 1; mI <= degree; ++mI)
		{
			mJ = mI + degree;
			result += (coefficients[mI] * vector[mI]) + (coefficients[mJ] * vector[mJ]);
		}
	}
	else
	{
		result = 0;
	}
}

int ViFourierPolynomial::maxCoefficients(const int &degree) const
{
	return (degree * 2) + 1; // a0, a1 ... an and b1 ... bn
}

ViFourierPolynomial* ViFourierPolynomial::clone() const
{
	return new ViFourierPolynomial(*this);
}
