#ifndef VICOSINEINTERPOLATOR_H
#define VICOSINEINTERPOLATOR_H

#include <viinterpolator.h>

class ViCosineInterpolator : public ViInterpolator
{

	public:

		ViCosineInterpolator* clone();

	protected:

		bool interpolateSamples(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize);

};

#endif
