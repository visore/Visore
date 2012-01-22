#include "vilibrarydetector.h"

QList<QString> ViLibraryDetector::detectLibraries(QString dirPath)
{
	return detectLibraries(QDir(dirPath));
}

QList<QString> ViLibraryDetector::detectLibraries(QDir dir)
{
	QStringList filters;
	#ifdef WIN32
		filters << "*.dll";
	#endif
	#ifdef APPLE
		filters << "*.dylib";
	#endif
	#ifdef UNIX
		filters << "*.so";
	#endif
	QFileInfoList list = dir.entryInfoList(filters, QDir::Files);
	QList<QString> result;
	for(int i = 0; list.length(); ++i)
	{
		result.append(list[i].absoluteFilePath());
	}
	return result;
}
