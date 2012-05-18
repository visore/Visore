#ifndef VIHAMMINGWINDOWER_H
#define VIHAMMINGWINDOWER_H

#include "viwindower.h"

class ViHammingWindower : public ViWindower
{
	public:
		static ViWindower* instance();
		qreal calculate(qint32 currentSample, qint32 totalSamples);
		QString name();

	protected:
		ViHammingWindower();
};

#endif
