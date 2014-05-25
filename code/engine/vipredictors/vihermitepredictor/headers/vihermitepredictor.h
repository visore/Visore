#ifndef VIHERMITEPREDICTOR_H
#define VIHERMITEPREDICTOR_H

#include <vipredictor.h>
#include <vipolynomial.h>

// http://www.cs.cornell.edu/~bindel/class/cs3220-s12/notes/lec19.pdf
// http://www3.nd.edu/~jjwteach/441/PdfNotes/lecture9.pdf

class ViHermitePredictor : public ViModelPredictor
{

	public:

		enum DerivativeMode
		{
			Outgoing,	// Uses the outgoing derivatives at a certain point. Eg: the derivative at point x1 is the derivative between x1 and x2
			Ingoing,	// Uses the ingoing derivatives at a certain point. Eg: the derivative at point x1 is the derivative between x0 and x1
			Average,		// Uses the average of the derivatives at both sides. Eg: the derivative at point x1 is the average of the derivatives between x0 and x1; and x1 and x2

			Default = Average
		};

	public:

		ViHermitePredictor(ViPolynomial *polynomial, const DerivativeMode &mode = Default, const Estimation &estimation = Fixed); // Takes ownership of the polynomial
		ViHermitePredictor(const ViHermitePredictor &other);
		~ViHermitePredictor();

		void setDerivativeMode(const DerivativeMode &mode);

		void setDegree(const int &degree, const int &degreeIndex = 0);

		void setParameter(const int &number, const qreal &value);

	protected:

		bool estimateModel(const int &degree, ViVector &coefficients, const qreal *samples, const int &size);
		void predictModel(const int &degree, const ViVector &coefficients, qreal *prediction, const int &size, const int &start);

		void calculateFirstDerivative(const qreal *samples, const int &size, qreal *derivatives);

		bool estimateModelOutgoing(const int &degree, ViVector &coefficients, const qreal *samples, const int &size);
		bool estimateModelIngoing(const int &degree, ViVector &coefficients, const qreal *samples, const int &size);
		bool estimateModelAverage(const int &degree, ViVector &coefficients, const qreal *samples, const int &size);

	private:

		bool (ViHermitePredictor::*estimatePointer)(const int &degree, ViVector &coefficients, const qreal *samples, const int &size);

	private:

		ViPolynomial *mPolynomial;
		DerivativeMode mDerivativeMode;

};

#endif
