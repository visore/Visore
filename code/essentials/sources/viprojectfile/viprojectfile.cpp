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
	mInfoPath = mProjectTempPath + QDir::separator() + mProjectName + ".sql";
	mDataPath = mProjectTempPath + QDir::separator() + "data";

	QDir dir(mProjectTempPath);
	if(!dir.exists())
	{
		if(!dir.mkpath(dir.absolutePath()))
		{
			return;
		}
	}

	dir = QDir(mDataPath);
	if(!dir.exists())
	{
		if(!dir.mkpath(dir.absolutePath()))
		{
			return;
		}
	}
}

QString ViProjectFile::filePath()
{
	return mFilePath;
}

QString ViProjectFile::infoPath()
{
	return mInfoPath;
}

QString ViProjectFile::dataPath()
{
	return mDataPath;
}

void ViProjectFile::load()
{
	mArchive.decompress(mProjectTempPath);
	ViDataDriver driver(mInfoPath);
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
	QFile file(mInfoPath);
	file.remove();
	ViDataDriver driver(mInfoPath);
	if(!driver.save(mProperties))
	{
		return;
	}
	QStringList files;
	files.append(mInfoPath);
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
