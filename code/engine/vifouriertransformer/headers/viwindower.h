#ifndef VIWINDOWER_H
#define VIWINDOWER_H

#include <QtGlobal>
#include <QString>
#include <QVector>
#include "math.h"
#include <qmath.h>

typedef QVector<double> ViWindow;

class ViWindower
{

	public:

		ViWindower();
		~ViWindower();
		void apply(float *data, int size);
		virtual double calculate(int currentSample, int totalSamples) = 0;
		virtual QString name() = 0;

	protected:

		void createWindow(int size);
		void fillWindow(int size);

	protected:

		ViWindow *mWindow;

};

#endif
