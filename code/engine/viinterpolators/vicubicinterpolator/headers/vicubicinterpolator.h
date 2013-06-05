#ifndef VICUBICINTERPOLATOR_H
#define VICUBICINTERPOLATOR_H

#include <viinterpolator.h>

/*
	http://paulbourke.net/miscellaneous/interpolation/
*/

class ViCubicInterpolator : public ViInterpolator
{

	public:

		ViCubicInterpolator();

		qreal calculate();
		ViSampleChunk calculate(int samples);

		ViCubicInterpolator* clone();

};

#endif
