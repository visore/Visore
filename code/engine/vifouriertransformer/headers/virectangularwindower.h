#ifndef VIRECTANGULARWINDOWER_H
#define VIRECTANGULARWINDOWER_H

#include "viwindower.h"

class ViRectangularWindower : public ViWindower
{

		double calculate(int currentSample, int totalSamples);
		QString name();

};

#endif
