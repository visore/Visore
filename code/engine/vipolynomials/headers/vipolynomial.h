#ifndef VIPOLYNOMIAL_H
#define VIPOLYNOMIAL_H

#include <vimatrix.h>
#include <QtMath>
#include <QVector>

class ViPolynomial
{

	public:

		ViPolynomial();
		ViPolynomial(const ViPolynomial &other);
		~ViPolynomial();

		void setDerivatives(const int &derivatives); // Specify how many derivatives (first, second, etc) should be precalculated

		virtual void setDegree(const int &degree); // Returns the maximum allowed window size (since Newton must have the same degree and window size
		const int& degree() const;

		const int& coefficientCount() const;

		void get(const int &degree, const int &x, ViVector &row, const int &derivative = 0);
		const ViVector* get(const int &degree, const int &derivative = 0);
		const ViVector* getOffset(const int &degree, const int &x, const int &derivative = 0);

		void solve(const ViVector &coefficients, const int &x, qreal &result);

		virtual void solve(const int &degree, const int &offset, const ViVector &coefficients, qreal &result, const int &derivative = 0) = 0;

		virtual ViPolynomial* clone() const = 0;

	protected:

		void createCache();

		virtual void construct(const int &degree, const qreal &x, const QVector<qreal> &allX, ViVector &row, const int &derivative = 0) = 0; // If derivative equal to 0, the normal polynomial will be constructed

		virtual int maxCoefficients(const int &degree) const = 0;

	protected:

		int mDegree;
		int mCoefficients;
		int mDerivatives;
		QList<QVector<QVector<ViVector>>> mCache;

};

#endif
