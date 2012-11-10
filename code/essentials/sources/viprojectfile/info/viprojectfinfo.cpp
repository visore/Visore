#include "viprojectinfo.h"
#include <QDir>

ViProjectInfo::ViProjectInfo()
{
	mFilePath = "";
	setNull(true);
	setName("Project");
}

void ViProjectInfo::setDirectory(QString directory)
{
	mFilePath = directory + QDir::separator() + mName + ".vml";
}

QString ViProjectInfo::name()
{
	return mName;
}

bool ViProjectInfo::isNull()
{
	return mIsNull;
}

bool ViProjectInfo::load()
{
	bool success = false;
	QFile file(mFilePath);
	if(file.open(QIODevice::ReadOnly))
	{
		ViElement document;
		document.fromXml(file.readAll());
		success = fromXml(document);
		file.close();
	}
	if(success)
	{
		setNull(true);
	}
	else
	{
		setNull(false);
	}
	return success;
}

bool ViProjectInfo::save()
{
	bool success = false;
	QFile file(mFilePath);
	if(file.open(QIODevice::Truncate | QIODevice::WriteOnly))
	{
		success = (file.write(toXml().toXml().toAscii()) > 0);
		file.close();
	}
	if(success)
	{
		setNull(true);
	}
	else
	{
		setNull(false);
	}
	return success;
}

void ViProjectInfo::setName(QString name)
{
	mName = name;
}

void ViProjectInfo::setNull(bool null)
{
	mIsNull = null;
}
