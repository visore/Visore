#ifndef VIRECTANGULARWINDOWER_H
#define VIRECTANGULARWINDOWER_H

#include "viwindower.h"

class ViRectangularWindower : public ViWindower
{
	public:
		static ViWindower* instance();
		qreal calculate(qint32 currentSample, qint32 totalSamples);
		QString name();

	protected:
		ViRectangularWindower();
};

#endif
