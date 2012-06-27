#include "viproject.h"

ViProject::ViProject(QString filePath)
	: QObject()
{
	mTempPath = "";
	mDataPath = "";
	setFilePath(filePath);

	mProjectFiles.append(&mProperties);
}

void ViProject::setFilePath(QString filePath)
{
	mArchive.setFilePath(filePath);
}

QString ViProject::filePath()
{
	return mArchive.filePath();
}

void ViProject::load()
{
	createTempStructure();
	QObject::connect(&mArchive, SIGNAL(finished()), this, SLOT(readInfo()));
	mArchive.decompress(mTempPath);
}

void ViProject::save()
{
	createTempStructure();
	for(int i = 0; i < mProjectFiles.size(); ++i)
	{
		mProjectFiles[i]->save();
	}
	QObject::connect(&mArchive, SIGNAL(finished()), this, SLOT(readInfo()));
	mArchive.compress(mTempPath);
}

void ViProject::readInfo()
{
	QObject::disconnect(&mArchive, SIGNAL(finished()), this, SLOT(readInfo()));
	for(int i = 0; i < mProjectFiles.size(); ++i)
	{
		mProjectFiles[i]->load();
	}
	emit finished();
}

bool ViProject::createTempStructure()
{
	mTempPath = ViManager::tempPath() + QDir::separator() + "projects" + QDir::separator() + id();
	mDataPath = mTempPath + QDir::separator() + "data";
	for(int i = 0; i < mProjectFiles.size(); ++i)
	{
		mProjectFiles[i]->setDirectory(mTempPath);
	}
	QDir dir(mDataPath);
	if(!dir.exists())
	{
		return dir.mkpath(dir.absolutePath());
	}
	return true;
}

bool ViProject::removeTempStructure()
{
	QDir dir(mTempPath);
	return dir.removeRecursively();
}

ViVersion ViProject::createdVersion()
{
	return mProperties.createdVersion();
}

ViVersion ViProject::editedVersion()
{
	return mProperties.editedVersion();
}
