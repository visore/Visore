#ifndef VILINEARINTERPOLATOR_H
#define VILINEARINTERPOLATOR_H

#include <viinterpolator.h>

/*
	Adopted from Audacity - InterpolateAudio.cpp
*/

class ViLinearInterpolator : public ViInterpolator
{

	public:

		virtual ViLinearInterpolator* clone();

	protected:

		virtual bool interpolateSamples(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize);

};

#endif
