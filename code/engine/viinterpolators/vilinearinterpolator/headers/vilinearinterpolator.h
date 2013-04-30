#ifndef VILINEARINTERPOLATOR_H
#define VILINEARINTERPOLATOR_H

#include <viinterpolator.h>

class ViLinearInterpolator : public ViInterpolator
{

	public:

		qreal calculate();
		ViSampleChunk calculate(int samples);

		ViInterpolator* clone();

};

#endif
