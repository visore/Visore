#ifndef VISPLINEINTERPOLATOR_H
#define VISPLINEINTERPOLATOR_H

#include <viinterpolator.h>

class ViSplineInterpolator : public ViAutoDegreeInterpolator
{

	public:

		ViSplineInterpolator();
		ViSplineInterpolator(const int &degree);

		virtual ViSplineInterpolator* clone();

	protected:

		virtual bool interpolateSamples(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize);
		inline qreal calculateMultiplier(const int &derivative, const int &parameterNumber);

};

#endif
