#ifndef VISPECTRUMWINDOW_H
#define VISPECTRUMWINDOW_H

#include <QList>
#include <QtGlobal>
#include "viwindowfunction.h"

class ViSpectrumWindow : public QList<qreal>
{
	public:
		ViSpectrumWindow(qint64 numberOfSamples, ViWindowFunction *function);
};

#endif
