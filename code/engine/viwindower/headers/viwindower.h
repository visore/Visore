#ifndef VIWINDOWER_H
#define VIWINDOWER_H

#include <QtGlobal>
#include <QString>
#include <QVector>
#include "visingleton.h"
#include "math.h"
#include <qmath.h>

typedef QVector<qreal> ViWindow;

class ViWindower : public ViSingleton
{
	public:
		~ViWindower();
		static ViWindower* instance();
		void apply(float *data, qint32 size);
		virtual qreal calculate(qint32 currentSample, qint32 totalSamples) = 0;
		virtual QString name() = 0;

	protected:
		ViWindower();
		void createWindow(qint32 size);
		void fillWindow(qint32 size);

	protected:
		ViWindow *mWindow;
		static QSharedPointer<ViWindower> mInstance;
};

#endif
