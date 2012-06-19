#include "vidatadriver.h"
#include <QSqlRecord>
#include <QSqlField>

void ViDataDriver::save(ViAbstractInfo &info)
{
	save(info.toInfo());
}

void ViDataDriver::save(ViProjectInfo &info)
{
	QSqlQuery query = insertQuery(info);
	query.exec();
}

void ViDataDriver::load(ViAbstractInfo &info)
{
	ViProjectInfo projectInfo;
	load(projectInfo);
	info.fromInfo(projectInfo);
}

void ViDataDriver::load(ViProjectInfo &info)
{
	QSqlQuery query = selectQuery(info);
	query.exec();
	QSqlRecord record = query.record();
	QSqlField field;
	for(int i = 0; i < record.count(); ++i)
	{
		field = record.field(i);
		info[field.name()] = field.value();
	}
}

ViProjectInfo ViDataDriver::load()
{
	ViProjectInfo projectInfo;
	load(projectInfo);
	return projectInfo;
}

QSqlQuery ViDataDriver::insertQuery(ViProjectInfo &info)
{
	QList<QString> keys = info.keys();
	QList<QVariant> values = info.values();
	int size = keys.size() - 1;
	QString idString = "";
	QString valueString = "";
	for(int i = 0; i < size; ++i)
	{
		idString += keys[i] + ", ";
		idString += "?, ";
	}
	if(size > 0)
	{
		idString += keys[size];
		idString += "?";
	}

	QSqlQuery query(mDatabase);
	query.prepare("INSERT INTO " + info.name() + "(" + idString + ") VALUES (" + valueString + ")");
	size = values.size();
	for(int i = 0; i < size; ++i)
	{
		query.addBindValue(values[i]);
	}
	return query;
}

QSqlQuery ViDataDriver::selectQuery(ViProjectInfo &info)
{
	QSqlQuery query(mDatabase);
	query.prepare("SELECT * FROM " + info.name());
	return query;
}
