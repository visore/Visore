#ifndef VIHANNWINDOWFUNCTION_H
#define VIHANNWINDOWFUNCTION_H

#include "viwindowfunction.h"
#include "math.h"
#include <qmath.h>

class ViHannWindowFunction : public ViWindowFunction
{
	public:
		static ViWindowFunction* instance();
		qreal calculate(qreal currentSample, qint32 totalSamples);
		QString name();

	protected:
		ViHannWindowFunction();
};

#endif
