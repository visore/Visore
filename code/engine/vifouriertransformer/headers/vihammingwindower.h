#ifndef VIHAMMINGWINDOWER_H
#define VIHAMMINGWINDOWER_H

#include "viwindower.h"

class ViHammingWindower : public ViWindower
{

	public:

		double calculate(int currentSample, int totalSamples);
		QString name();

};

#endif
