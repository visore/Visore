#include "viprojectfile.h"
#include <QDir>

ViProjectFile::ViProjectFile()
{
	mFilePath = "";
	setNull(true);
	setName("Project");
}

void ViProjectFile::setDirectory(QString directory)
{
	mFilePath = directory + QDir::separator() + mName + ".vml";
}

QString ViProjectFile::name()
{
	return mName;
}

bool ViProjectFile::isNull()
{
	return mIsNull;
}

bool ViProjectFile::load()
{
	bool success = false;
	QFile file(mFilePath);
	if(file.open(QIODevice::ReadOnly))
	{
		QDomDocument document;
		if(document.setContent(file.readAll(), false))
		{
			fromXml(document);
			success = true;
		}
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

bool ViProjectFile::save()
{
	bool success = false;
	QFile file(mFilePath);
	if(file.open(QIODevice::WriteOnly))
	{
		success = (file.write(toXml().toByteArray()) > 0);
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

void ViProjectFile::setName(QString name)
{
	mName = name;
}

void ViProjectFile::setNull(bool null)
{
	mIsNull = null;
}
