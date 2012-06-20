#ifndef VIDATADRIVER_H
#define VIDATADRIVER_H

#include "vipropertiesinfo.h"
#include <QMap>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>

typedef QMap<QString, QVariant> ViInfoMap;

class ViDataDriver
{

	public:

		ViDataDriver(QString filePath);
		bool save(ViPropertiesInfo &info);
		bool load(ViPropertiesInfo &info);

	protected:

		int versionId(int major, int minor, int patch);

		QSqlError create(QString name);
		QSqlError insert(QString name, ViInfoMap &map);
		QSqlError select(QString queryString, ViInfoMap &map);
		QSqlError select(QString queryString, QList<ViInfoMap> &mapList);
		bool contains(QString queryString);
		int lastId(QString name = "");

	protected:

		QSqlDatabase mDatabase;

};

#endif
