#ifndef VIHANNWINDOWER_H
#define VIHANNWINDOWER_H

#include "viwindower.h"

class ViHannWindower : public ViWindower
{

	public:

		double calculate(int currentSample, int totalSamples);
		QString name();

};

#endif
