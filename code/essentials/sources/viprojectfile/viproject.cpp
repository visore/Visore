#include "viproject.h"

#define RECORDING_NAME_LENGTH 5

ViProject::ViProject(QString filePath)
	: QObject()
{
	mTempPath = "";
	mDataPath = "";
	mOriginalPath = "";
	mCorrectedPath = "";
	setFilePath(filePath);

	mProjectFiles.append(&mProperties);
}

ViProject::~ViProject()
{
	QDir dir(mTempPath);
	dir.removeRecursively();
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
	mArchive.decompressData(mTempPath);
}

void ViProject::save()
{
	createTempStructure();
	for(int i = 0; i < mProjectFiles.size(); ++i)
	{
		mProjectFiles[i]->save();
	}
	QObject::connect(&mArchive, SIGNAL(finished()), this, SLOT(readInfo()));
	mArchive.compressData(mTempPath);
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
	mOriginalPath = mDataPath + QDir::separator() + "original";
	mCorrectedPath = mDataPath + QDir::separator() + "corrected";

	for(int i = 0; i < mProjectFiles.size(); ++i)
	{
		mProjectFiles[i]->setDirectory(mTempPath);
	}
	QDir originalDir(mOriginalPath);
	QDir correctedDir(mCorrectedPath);
	return originalDir.mkpath(originalDir.absolutePath()) && correctedDir.mkpath(correctedDir.absolutePath());
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

QString ViProject::nextSongName()
{
	QDir originalDir(mOriginalPath);
	QDir correctedDir(mCorrectedPath);
	QFileInfoList originalFiles = originalDir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
	QFileInfoList correctedFiles = correctedDir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);

	QStringList originalNames;
	QStringList correctedNames;
	for(int i = 0; i < originalFiles.size(); ++i)
	{
		originalNames.append(originalFiles[i].baseName());
	}

	for(int i = 0; i < correctedFiles.size(); ++i)
	{
		correctedNames.append(correctedFiles[i].baseName());
	}

	int number = 1;
	QString name = padName(number);
	while(originalNames.contains(name) || correctedNames.contains(name))
	{
		++number;
		name = padName(number);
	}
	return name;
}

QString ViProject::originalPath()
{
	return mOriginalPath;
}

QString ViProject::correctedPath()
{
	return mCorrectedPath;
}

QString ViProject::padName(int number)
{
	QString result = QString::number(number);
	int padding = RECORDING_NAME_LENGTH - result.length();
	for(int i = 0; i < padding; ++i)
	{
		result = "0" + result;
	}
	return result;
}
