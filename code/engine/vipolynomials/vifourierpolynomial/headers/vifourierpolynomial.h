#ifndef VIFOURIERPOLYNOMIAL_H
#define VIFOURIERPOLYNOMIAL_H

#include <vipolynomial.h>

class ViFourierPolynomial : public ViPolynomial
{

	public:

		ViFourierPolynomial();
		ViFourierPolynomial(const ViFourierPolynomial &other);
		~ViFourierPolynomial();

		ViFourierPolynomial* clone() const;

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
