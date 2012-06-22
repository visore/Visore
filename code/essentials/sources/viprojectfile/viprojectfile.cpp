#include "viprojectfile.h"
#include "viarchive.h"

#include "vilogger.h"

ViProjectFile::ViProjectFile(QString filePath)
	: QObject()
{
	QObject::connect(&mArchive, SIGNAL(finished()), this, SIGNAL(finished()));
	mArchive.setComment("Visore Project");
	setFilePath(filePath);
}

void ViProjectFile::setFilePath(QString filePath)
{
	QString oldName = fileName();
	mFilePath = filePath;
	QString extension = "." + ViManager::projectExtension();
	if(!mFilePath.endsWith(extension))
	{
		mFilePath += extension;
	}
	mArchive.setFilePath(mFilePath);
	mProjectName = fileName();
	mProjectTempPath = ViManager::tempPath() + QDir::separator() + "projects" + QDir::separator() + mProperties.id();
}

QString ViProjectFile::filePath()
{
	return mFilePath;
}

void ViProjectFile::load()
{
	mArchive.decompress(mProjectTempPath);
	QString tempFilePath = mProjectTempPath + QDir::separator() + mProjectName + ".sql";
	ViDataDriver driver(tempFilePath);
	if(!driver.load(mProperties))
	{
		return;
	}
	QDir dir(mProjectTempPath);
	QString newName = ViManager::tempPath() + QDir::separator() + "projects" + QDir::separator() + mProperties.id();
	QDir newDir(newName);
	if(newDir.exists())
	{
		newDir.removeRecursively();
	}
	if(dir.rename(mProjectTempPath, newName))
	{
		mProjectTempPath = newName;
	}
	else
	{
		dir.removeRecursively();
	}
}

void ViProjectFile::save()
{
	if(mProperties.isNull())
	{
		mProperties.initializeCurrent();
		mProjectTempPath = ViManager::tempPath() + QDir::separator() + "projects" + QDir::separator() + mProperties.id();
	}
	QDir dir(mProjectTempPath);
	if(!dir.exists())
	{
		if(!dir.mkpath(dir.absolutePath()))
		{
			return;
		}
	}
	QString tempFilePath = mProjectTempPath + QDir::separator() + mProjectName + ".sql";
	QFile file(tempFilePath);
	file.remove();
	ViDataDriver driver(tempFilePath);
	if(!driver.save(mProperties))
	{
		return;
	}
	QStringList files;
	files.append(tempFilePath);
	mArchive.compress(files);
}

QString ViProjectFile::fileName()
{
	if(mFilePath == "")
	{
		return "";
	}
	int start = mFilePath.lastIndexOf(QDir::separator()) + 1;
	int end = mFilePath.indexOf(".", start);
	return mFilePath.mid(start, end - start);
}

ViPropertiesInfo& ViProjectFile::properties()
{
	return mProperties;
}

QString ViProjectFile::toString()
{
	QString result = "******************************************\nVisore Project File\n******************************************\n";
	result += mProperties.toString();
	result += "\n******************************************";
	return result;
}
