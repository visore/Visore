#ifndef VILINEARINTERPOLATOR_H
#define VILINEARINTERPOLATOR_H

#include <viinterpolator.h>

/*
	http://paulbourke.net/miscellaneous/interpolation/
*/

class ViLinearInterpolator : public ViInterpolator
{

	public:

		ViLinearInterpolator();

		qreal calculate();
		ViSampleChunk calculate(int samples);

		ViLinearInterpolator* clone();

};

#endif
