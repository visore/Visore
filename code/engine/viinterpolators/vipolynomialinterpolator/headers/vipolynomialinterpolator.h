#ifndef VIPOLYNOMIALINTERPOLATOR_H
#define VIPOLYNOMIALINTERPOLATOR_H

#include <viinterpolator.h>

class ViPolynomialInterpolator : public ViInterpolator
{

	public:

		ViPolynomialInterpolator();

		// Eg: degree-2 = quadratic
		// Eg: degree-3 = qubic
		void setDegree(const int &degree);
		int degree();

		virtual ViPolynomialInterpolator* clone();

	protected:

		virtual bool interpolateSamples(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize);

	private:

		int mDegree;

};

#endif
