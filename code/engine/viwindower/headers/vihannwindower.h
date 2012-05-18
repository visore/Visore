#ifndef VIHANNWINDOWER_H
#define VIHANNWINDOWER_H

#include "viwindower.h"

class ViHannWindower : public ViWindower
{
	public:
		static ViWindower* instance();
		qreal calculate(qint32 currentSample, qint32 totalSamples);
		QString name();

	protected:
		ViHannWindower();
};

#endif
