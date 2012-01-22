#ifndef VILIBRARYDETECTOR_H
#define VILIBRARYDETECTOR_H

#include "vierror.h"
#include <QList>
#include <QFileInfoList>
#include <QDir>

class ViLibraryDetector : public ViError
{
	public:
		static QList<QString> detectLibraries(QString dirPath);
		static QList<QString> detectLibraries(QDir dir);
};

#endif

