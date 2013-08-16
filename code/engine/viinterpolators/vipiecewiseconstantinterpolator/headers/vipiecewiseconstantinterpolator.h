#ifndef VIPIECEWISECONSTANTINTERPOLATOR_H
#define VIPIECEWISECONSTANTINTERPOLATOR_H

#include <viinterpolator.h>

class ViPiecewiseConstantInterpolator : public ViInterpolator
{

	public:

		virtual ViPiecewiseConstantInterpolator* clone();

	protected:

		virtual bool interpolateSamples(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize);

};

#endif
