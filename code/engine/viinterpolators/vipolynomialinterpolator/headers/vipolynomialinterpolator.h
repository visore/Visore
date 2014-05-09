#ifndef VIPOLYNOMIALINTERPOLATOR_H
#define VIPOLYNOMIALINTERPOLATOR_H

#include <viinterpolator.h>

class ViPolynomialInterpolator : public ViDegreeInterpolator
{

	public:

		ViPolynomialInterpolator();
		ViPolynomialInterpolator(const int &degree);

		virtual ViPolynomialInterpolator* clone();

		void setParameters(const qreal &param1, const qreal &param2); // param1 = window size, param2 = degree

	protected:

		virtual bool interpolateSamples(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize);

		bool solveEquations(double **matrix, double *coefficients, const int &degree);

};

#endif
