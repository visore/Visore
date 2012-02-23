#ifndef VINOWINDOWFUNCTION_H
#define VINOWINDOWFUNCTION_H

#include "viwindowfunction.h"
#include "math.h"

class ViNoWindowFunction : public ViWindowFunction
{
	public:
		static ViWindowFunction* instance();
		qreal calculate(qreal currentSample, qint32 totalSamples);
		QString name();

	protected:
		ViNoWindowFunction();
};

#endif
