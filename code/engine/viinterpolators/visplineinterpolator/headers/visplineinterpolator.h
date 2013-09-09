#ifndef VISPLINEINTERPOLATOR_H
#define VISPLINEINTERPOLATOR_H

#include <viinterpolator.h>

class ViSplineInterpolator : public ViInterpolator
{

	public:

		ViSplineInterpolator();

		// Eg: degree-2 = quadratic
		// Eg: degree-3 = qubic
		void setDegree(const int &degree);
		int degree();

		virtual ViSplineInterpolator* clone();

	protected:

		virtual bool interpolateSamples(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize);
		inline qreal calculateMultiplier(const int &derivative, const int &parameterNumber);

	private:

		int mDegree;

};

#endif