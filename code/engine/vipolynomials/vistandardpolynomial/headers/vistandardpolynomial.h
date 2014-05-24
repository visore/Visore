#ifndef VISTANDARDPOLYNOMIAL_H
#define VISTANDARDPOLYNOMIAL_H

#include <vipolynomial.h>

class ViStandardPolynomial : public ViPolynomial
{

	public:

		ViStandardPolynomial();
		ViStandardPolynomial(const ViStandardPolynomial &other);
		~ViStandardPolynomial();

		ViStandardPolynomial* clone() const;

	protected:

		void construct(const int &degree, const qreal &x, const QVector<qreal> &allX, ViVector &row, const int &derivative = 0);
		void solve(const int &degree, const int &offset, const ViVector &coefficients, qreal &result, const int &derivative = 0);

		int maxCoefficients(const int &degree) const;

	private:

		int mI;

};

#endif
