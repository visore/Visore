#ifndef VICOSINEINTERPOLATOR_H
#define VICOSINEINTERPOLATOR_H

#include <viinterpolator.h>

class ViCosineInterpolator : public ViInterpolator
{

	public:

		qreal calculate();
		ViSampleChunk calculate(int samples);

		ViCosineInterpolator* clone();

};

#endif