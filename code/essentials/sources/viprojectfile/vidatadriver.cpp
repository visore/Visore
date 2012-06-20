#include "vidatadriver.h"
#include <QSqlRecord>
#include "vilogger.h"

ViDataDriver::ViDataDriver(QString filePath)
{
	mDatabase = QSqlDatabase::addDatabase("QSQLITE");
	mDatabase.setDatabaseName(filePath);
}

bool ViDataDriver::save(ViPropertiesInfo &info)
{
	if(mDatabase.open())
	{
		create("version");
		int createdId = versionId(info.createdVersion().major(), info.createdVersion().minor(), info.createdVersion().patch());
		if(createdId < 1)
		{
			return false;
		}
		int accessedId = versionId(info.accessedVersion().major(), info.accessedVersion().minor(), info.accessedVersion().patch());
		if(accessedId < 1)
		{
			return false;
		}
		int editedId = versionId(info.editedVersion().major(), info.editedVersion().minor(), info.editedVersion().patch());
		if(editedId < 1)
		{
			return false;
		}

		create("properties");
		ViInfoMap map;
		map.insert("createddate", info.createdDateTime().toMSecsSinceEpoch());
		map.insert("createdversion", createdId);
		map.insert("accesseddate", info.accessedDateTime().toMSecsSinceEpoch());
		map.insert("accessedversion", accessedId);
		map.insert("editeddate", info.editedDateTime().toMSecsSinceEpoch());
		map.insert("editedversion", editedId);
		if(insert("properties", map).type() == QSqlError::NoError)
		{
			mDatabase.close();
			return true;
		}
		mDatabase.close();
	}
	return false;
}

bool ViDataDriver::load(ViPropertiesInfo &info)
{
	if(mDatabase.open())
	{
		
		ViInfoMap map;
		select("SELECT * FROM properties", map);
		if(map.size() == 0)
		{
			return false;
		}
		info.setCreatedDateTime(QDateTime::fromMSecsSinceEpoch(map["createddate"].toDouble()));
		info.setAccessedDateTime(QDateTime::fromMSecsSinceEpoch(map["accesseddate"].toDouble()));
		info.setEditedDateTime(QDateTime::fromMSecsSinceEpoch(map["editeddate"].toDouble()));

		ViInfoMap version;
		select("SELECT * FROM version WHERE id = " + map["createdversion"].toString(), version);
		if(version.size() == 0)
		{
			return false;
		}
		info.setCreatedVersion(ViVersion(version["major"].toInt(), version["minor"].toInt(), version["patch"].toInt()));

		version.clear();
		select("SELECT * FROM version WHERE id = " + map["accessedversion"].toString(), version);
		if(version.size() == 0)
		{
			return false;
		}
		info.setAccessedVersion(ViVersion(version["major"].toInt(), version["minor"].toInt(), version["patch"].toInt()));

		version.clear();
		select("SELECT * FROM version WHERE id = " + map["editedversion"].toString(), version);
		if(version.size() == 0)
		{
			return false;
		}
		info.setEditedVersion(ViVersion(version["major"].toInt(), version["minor"].toInt(), version["patch"].toInt()));
cout<<info.toString().toAscii().data()<<endl;
		mDatabase.close();
		return true;
	}
	return false;
}

int ViDataDriver::versionId(int major, int minor, int patch)
{
	int id = -1;
	ViInfoMap map;
	select("SELECT id FROM version WHERE major = " + QString::number(major) + " AND minor = " + QString::number(minor) + " AND patch = " + QString::number(patch), map);
	if((id = map["id"].toInt()) > 0)
	{
		id = map["id"].toInt();
	}
	else
	{
		map.clear();
		map.insert("major", major);
		map.insert("minor", minor);
		map.insert("patch", patch);
		if(insert("version", map).type() == QSqlError::NoError)
		{
			id = lastId();
		}
	}
	return id;
}

QSqlError ViDataDriver::create(QString name)
{
	QSqlQuery query(mDatabase);
	if(name.toLower() == "properties")
	{
		query.prepare("CREATE TABLE IF NOT EXISTS " + name.toLower() + "( id INTEGER PRIMARY KEY, createddate DATETIME, createdversion INTEGER, accesseddate DATETIME, accessedversion INTEGER, editeddate DATETIME, editedversion INTEGER, FOREIGN KEY(createdversion) REFERENCES version(id), FOREIGN KEY(editedversion) REFERENCES version(id), FOREIGN KEY(editedversion) REFERENCES version(id) );");
	}
	else if(name.toLower() == "version")
	{
		query.prepare("CREATE TABLE IF NOT EXISTS version ( id INTEGER PRIMARY KEY, major INTEGER, minor INTEGER, patch INTEGER );");
	}
	query.exec();
	return query.lastError();
}

QSqlError ViDataDriver::insert(QString name, ViInfoMap &map)
{
	QList<QString> keys = map.keys();
	QList<QVariant> values = map.values();
	int size = keys.size() - 1;
	QString idString = "";
	QString valueString = "";
	for(int i = 0; i < size; ++i)
	{
		idString += keys[i] + ", ";
		valueString += "?, ";
	}
	if(size > 0)
	{
		idString += keys[size];
		valueString += "?";
	}

	QSqlQuery query(mDatabase);
	query.prepare("INSERT INTO " + name + "(" + idString + ") VALUES (" + valueString + ")");
	size = values.size();
	for(int i = 0; i < size; ++i)
	{
		query.addBindValue(values[i]);
	}
	query.exec();
	return query.lastError();
}

QSqlError ViDataDriver::select(QString queryString, ViInfoMap &map)
{
	QSqlQuery query(mDatabase);
	query.prepare(queryString);
	query.exec();
	if(query.next())
	{
		QSqlRecord record = query.record();
		for(int i = 0; i < record.count(); ++i)
		{
			map.insert(record.fieldName(i), query.value(i));
		}
	}
	return query.lastError();
}

QSqlError ViDataDriver::select(QString queryString, QList<ViInfoMap> &mapList)
{
	QSqlQuery query(mDatabase);
	query.prepare(queryString);
	query.exec();
	QSqlRecord record = query.record();
	while(query.next())
	{
		ViInfoMap map;
		for(int i = 0; i < record.count(); ++i)
		{
			map.insert(record.fieldName(i), query.value(i));
		}
		mapList.append(map);
	}
	return query.lastError();
}

bool ViDataDriver::contains(QString queryString)
{
	ViInfoMap map;
	select(queryString, map);
	if(map.size() > 0)
	{
		return true;
	}
	return false;
}

int ViDataDriver::lastId(QString name)
{
	ViInfoMap map;
	select("SELECT last_insert_rowid()", map);
	return map.values()[0].toInt();
}
