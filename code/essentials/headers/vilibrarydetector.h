#ifndef VILIBRARYDETECTOR_H
#define VILIBRARYDETECTOR_H

#include <QList>
#include <QFileInfoList>
#include <QDir>

class ViLibraryDetector
{

	public:

		static QList<QString> detectLibraries(QString dirPath);
		static QList<QString> detectLibraries(QDir dir);

};

#endif
