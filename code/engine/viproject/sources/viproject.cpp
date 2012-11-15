#include "viproject.h"
#include "viaudiocodec.h"

#include "vilogger.h"

#define RECORDING_NAME_LENGTH 2

/*******************************************************************************************************************

			CONSTRUCTORS & DESTRUCTORS

*******************************************************************************************************************/

ViProject::ViProject(QString filePath)
	: QObject(), ViId()
{
	setProjectName("");
	setFilePath(filePath);

	mSides = 0;
	mCurrentSide = 1;
	mCurrentTrack = 0;

	QObject::connect(&mEncoder, SIGNAL(finished()), this, SLOT(save()));

	save();
}

ViProject::ViProject(QString projectName, QString filePath, int sides)
	: QObject(), ViId()
{
	setProjectName(projectName);
	setFilePath(filePath);
	setSides(sides);

	mCurrentSide = 1;
	mCurrentTrack = 0;

	save();
}

ViProject::~ViProject()
{
	removeTempStructure();
}

void ViProject::serialize(ViAudioObjectPointer object)
{
t= object;
	++mCurrentTrack;
	QString path = generateFileName(object->songInfo(), mPaths["data_original"]);
	object->setOriginalFile(path);
	mEncoder.encode(object->originalBuffer(), path, mFormat, 0, object->songInfo());
LOG(path);
}

void ViProject::setFormat(ViAudioFormat format)
{
	mFormat = format;
}

/*******************************************************************************************************************

	BASICS

*******************************************************************************************************************/

qint64 ViProject::size()
{
	QFile file(filePath());
	return file.size();
}

void ViProject::setFilePath(QString filePath)
{
	mArchive.setFilePath(filePath);
}

QString ViProject::filePath()
{
	return mArchive.filePath();
}

void ViProject::setProjectName(QString name)
{
	//mProperties.setProjectName(name);
}

QString ViProject::projectName()
{
	//return mProperties.projectName();
}

/*******************************************************************************************************************

	SIDES

*******************************************************************************************************************/

void ViProject::setSides(int sides)
{
	mSides = sides;
	nextSide();
}

int ViProject::sides()
{
	return mSides;
}

int ViProject::currentSide()
{
	return mCurrentSide;
}

void ViProject::nextSide()
{
	mCurrentTrack = 0;
	++mCurrentSide;
}

/*******************************************************************************************************************

	LOAD & SAVE

*******************************************************************************************************************/

bool ViProject::load(bool minimal)
{
	createTempStructure();
	QObject::connect(&mArchive, SIGNAL(finished()), this, SLOT(readInfo()));
	if(minimal)
	{
		return mArchive.decompressData(mPaths["root"], mArchive.fileList(".vml"));
	}
	else
	{
		return mArchive.decompressData(mPaths["root"]);
	}
}

void ViProject::save()
{
	createTempStructure();
	//QObject::connect(&mArchive, SIGNAL(finished()), this, SLOT(readInfo()));
	mArchive.compressData(mPaths["root"]);
}

bool ViProject::createTempStructure()
{
	//Declare paths
	mPaths["root"] = ViManager::tempPath() + QDir::separator() + "projects" + QDir::separator() + id() + QDir::separator();

	mPaths["info"] = mPaths["root"] + "info" + QDir::separator();
	mPaths["info_general"] = mPaths["info"] + "general" + QDir::separator();
	mPaths["info_songs"] = mPaths["info"] + "songs" + QDir::separator();
	mPaths["info_correlations"] = mPaths["info"] + "correlations" + QDir::separator();
	
	mPaths["data"] = mPaths["root"] + "data" + QDir::separator();
	mPaths["data_target"] = mPaths["data"] + "target" + QDir::separator();
	mPaths["data_original"] = mPaths["data"] + "original" + QDir::separator();
	mPaths["data_corrected"] = mPaths["data"] + "corrected" + QDir::separator();
	mPaths["data_albumart"] = mPaths["data"] + "albumart" + QDir::separator();
	for(int i = 1; i <= mSides; ++i)
	{
		mPaths["data_target_side" + QString::number(i)] = mPaths["data_target"] + "side" + QString::number(i) + QDir::separator();
		mPaths["data_original_side" + QString::number(i)] = mPaths["data_original"] + "side" + QString::number(i) + QDir::separator();
		mPaths["data_corrected_side" + QString::number(i)] = mPaths["data_corrected"] + "side" + QString::number(i) + QDir::separator();
	}

	//Create paths
	bool success = true;
	for(QMap<QString,QString>::iterator iterator = mPaths.begin(); iterator != mPaths.end(); ++iterator)
	{
		QDir dir(iterator.value());
		success &= dir.mkpath(dir.absolutePath());
	}
	return success;
}

bool ViProject::removeTempStructure()
{
	QDir dir(mPaths["root"]);
	return dir.removeRecursively();
}

QString ViProject::generateFileName(ViSongInfo info, QString folder)
{
	// Create correct folder
	if(folder != "" && !folder.endsWith(QDir::separator()))
	{
		folder.append(QDir::separator());
	}
	folder += "side" + QString::number(mCurrentSide) + QDir::separator();

	// Create track number
	QString trackNumber = QString::number(mCurrentTrack);
	int padding = RECORDING_NAME_LENGTH - trackNumber.length();
	for(int i = 0; i < padding; ++i)
	{
		trackNumber = "0" + trackNumber;
	}
	folder += trackNumber + ". ";

	// Create track name
	if(info.artistName() == "")
	{
		folder += "Unknown Artist - ";
	}
	else
	{
		folder += info.artistName() + " - ";
	}
	if(info.songTitle() == "")
	{
		folder += "Unknown Title";
	}
	else
	{
		folder += info.songTitle();
	}

	// Create extension
	folder += mFormat.codec()->extension(".");
	
	return folder;
}
