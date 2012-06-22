#include "viprojectfile.h"
#include "viarchive.h"

#include "vilogger.h"

ViProjectFile::ViProjectFile(QString filePath)
	: QObject()
{
	QObject::connect(&mArchive, SIGNAL(finished()), this, SLOT(completeArchive()));
	mArchive.setComment("Visore Project");
	setFilePath(filePath);
}

void ViProjectFile::setFilePath(QString filePath)
{
	mFilePath = filePath;
	QString extension = "." + ViManager::projectExtension();
	if(!mFilePath.endsWith(extension))
	{
		mFilePath += extension;
	}
	mArchive.setFilePath(mFilePath);
	mProjectName = fileName();
	mProjectTempPath = ViManager::tempPath() + QDir::separator() + mProjectName;
}

QString ViProjectFile::filePath()
{
	return mFilePath;
}

void ViProjectFile::load()
{
	complete = &ViProjectFile::completeLoad;
}

void ViProjectFile::save()
{
	QDir dir(mProjectTempPath);
	if(!dir.exists())
	{
		if(!dir.mkpath(dir.absolutePath()))
		{
			return;
		}
	}
	QString tempFilePath = mProjectTempPath + QDir::separator() + mProjectName + ".sql";
	ViDataDriver driver(tempFilePath);
	if(!driver.save(mProperties))
	{
		return;
	}
	complete = &ViProjectFile::completeSave;
	QStringList files;
	files.append(tempFilePath);
	mArchive.compress(files);

}

void ViProjectFile::completeArchive()
{
	(this->*complete)();
	emit finished();
}
void ViProjectFile::completeSave()
{
	QDir dir(mProjectTempPath);
    dir.removeRecursively();
	
}

void ViProjectFile::completeLoad()
{

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
