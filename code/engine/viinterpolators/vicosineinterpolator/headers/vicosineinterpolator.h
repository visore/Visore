/*#ifndef VICOSINEINTERPOLATOR_H
#define VICOSINEINTERPOLATOR_H

#include <viinterpolator.h>

// http://paulbourke.net/miscellaneous/interpolation/

class ViCosineInterpolator : public ViInterpolator
{

	public:

		ViCosineInterpolator();

		ViCosineInterpolator* clone();

	protected:

		bool interpolate(const qreal *leftSamples, const int &leftSize, const qreal *rightSamples, const int &rightSize, qreal *outputSamples, const int &outputSize);

};

#endif
*/
