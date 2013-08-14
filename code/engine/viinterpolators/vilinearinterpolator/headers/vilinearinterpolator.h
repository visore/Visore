#ifndef VILINEARINTERPOLATOR_H
#define VILINEARINTERPOLATOR_H

#include <viinterpolator.h>

/*
	Adopted from Audacity - InterpolateAudio.cpp
*/

class ViLinearInterpolator : public ViInterpolator
{

	public:

		virtual ~ViLinearInterpolator();

		virtual ViLinearInterpolator* clone();

	protected:

		virtual bool interpolateSamples(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize);

		virtual qreal leftValue(const qreal *leftSamples, const int &leftSize);
		virtual qreal rightValue(const qreal *rightSamples, const int &rightSize);
		virtual qreal middleValue(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize);

		virtual qreal leftDelta(const qreal *leftSamples, const int &leftSize);
		virtual qreal rightDelta(const qreal *rightSamples, const int &rightSize);
		virtual qreal middleDelta(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, const int &outputSize);

};

#endif
