#ifndef VIPOLYNOMIALPREDICTOR_H
#define VIPOLYNOMIALPREDICTOR_H

#include <vipredictor.h>
#include <vipolynomial.h>

class ViPolynomialPredictor : public ViModelPredictor
{

	public:

		ViPolynomialPredictor(ViPolynomial *polynomial, const Estimation &estimation = Fixed); // Takes ownership of the polynomial
		ViPolynomialPredictor(const ViPolynomialPredictor &other);
		~ViPolynomialPredictor();

		void setDegree(const int &degree, const int &degreeIndex = 0);

		void setParameter(const int &number, const qreal &value);

	protected:

		bool estimateModel(const int &degree, ViVector &coefficients, const qreal *samples, const int &size);
		void predictModel(const int &degree, const ViVector &coefficients, qreal *prediction, const int &size, const int &start);

	private:

		ViPolynomial *mPolynomial;

};

#endif
