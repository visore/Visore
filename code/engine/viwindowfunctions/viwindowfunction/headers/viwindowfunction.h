#ifndef VIWINDOWFUNCTION_H
#define VIWINDOWFUNCTION_H

#include <QtGlobal>
#include <QString>
#include "visingleton.h"

class ViWindowFunction : public ViSingleton
{
	public:
		static ViWindowFunction* instance();
		virtual qreal calculate(qreal currentSample, qint32 totalSamples) = 0;
		virtual QString name() = 0;

	protected:
		ViWindowFunction();

	protected:
		static QSharedPointer<ViWindowFunction> mInstance;
};

#endif
