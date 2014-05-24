#ifndef VINEWTONPOLYNOMIAL_H
#define VINEWTONPOLYNOMIAL_H

#include <vipolynomial.h>

class ViNewtonPolynomial : public ViPolynomial
{

	public:

		ViNewtonPolynomial();
		ViNewtonPolynomial(const ViNewtonPolynomial &other);
		~ViNewtonPolynomial();

		ViNewtonPolynomial* clone() const;

	protected:

		void construct(const int &degree, const qreal &x, const QVector<qreal> &allX, ViVector &row, const int &derivative = 0);
		void solve(const int &degree, const int &offset, const ViVector &coefficients, qreal &result, const int &derivative = 0);

		int maxCoefficients(const int &degree) const;

	private:

		int mI;
		int mJ;
		qreal mValue1;
		qreal mValue2;
		qreal mValue3;

};

#endif
