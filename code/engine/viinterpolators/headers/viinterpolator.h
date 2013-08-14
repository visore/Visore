#ifndef VIINTERPOLATOR_H
#define VIINTERPOLATOR_H

#include <vilibrary.h>
#include <vinoise.h>

class ViInterpolator : public ViLibrary
{

	public:

		ViInterpolator();
		ViInterpolator(const ViInterpolator &other);
		virtual ~ViInterpolator();

		bool interpolate(ViSampleChunk &samples, const ViNoise &noise);

		virtual ViInterpolator* clone() = 0;

	protected:

		virtual bool interpolateSamples(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize) = 0;

};

#endif
