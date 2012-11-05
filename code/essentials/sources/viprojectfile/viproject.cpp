#include "viproject.h"

#define RECORDING_NAME_LENGTH 2

ViProject::ViProject(QString filePath, QString projectName)
	: QObject()
{
	mSideCount = 0;
	mCurrentSide = 0;
	mTempPath = "";
	mInfoPath = "";
	mDataPath = "";
	mOriginalPath = "";
	mCorrectedPath = "";
	mAlbumArtPath = "";
	setFilePath(filePath);
	setProjectName(projectName);

	mProjectInfos.append(&mProperties);
	mProjectInfos.append(&mSongs);
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

QString ViProject::fileName(bool withExtension)
{
	QFileInfo file(filePath());
	return file.fileName();
}

bool ViProject::load(bool minimal)
{
	createTempStructure();
	QObject::connect(&mArchive, SIGNAL(finished()), this, SLOT(readInfo()));
	if(minimal)
	{
		return mArchive.decompressData(mTempPath, mArchive.fileList(".vml"));
	}
	else
	{
		return mArchive.decompressData(mTempPath);
	}
}

void ViProject::save()
{
	createTempStructure();
	for(int i = 0; i < mProjectInfos.size(); ++i)
	{
		mProjectInfos[i]->save();
	}
	QObject::connect(&mArchive, SIGNAL(finished()), this, SLOT(readInfo()));
	mArchive.compressData(mTempPath);
}

qint64 ViProject::size()
{
	QFile file(filePath());
	return file.size();
}

int ViProject::sideCount()
{
	return mSideCount;
}

int ViProject::currentSide()
{
	return mCurrentSide;
}

void ViProject::setSideCount(int count)
{
	mSideCount = count;
}

bool ViProject::nextSide()
{
	if(mCurrentSide < mSideCount)
	{
		++mCurrentSide;
		return true;
	}
	return false;
}

void ViProject::readInfo()
{
	QObject::disconnect(&mArchive, SIGNAL(finished()), this, SLOT(readInfo()));
	for(int i = 0; i < mProjectInfos.size(); ++i)
	{
		mProjectInfos[i]->load();
	}
	emit finished();
}

bool ViProject::createTempStructure()
{
	mTempPath = ViManager::tempPath() + QDir::separator() + "projects" + QDir::separator() + id();
	mInfoPath = mTempPath + QDir::separator() + "info";
	mDataPath = mTempPath + QDir::separator() + "data";
	mOriginalPath = mDataPath + QDir::separator() + "original";
	mCorrectedPath = mDataPath + QDir::separator() + "corrected";
	mAlbumArtPath = mDataPath + QDir::separator() + "albumart";

	for(int i = 0; i < mProjectInfos.size(); ++i)
	{
		mProjectInfos[i]->setDirectory(mInfoPath);
	}
	QDir infoDir(mInfoPath);
	QDir originalDir(mOriginalPath);
	QDir correctedDir(mCorrectedPath);
	QDir albumArtDir(mAlbumArtPath);
	return infoDir.mkpath(infoDir.absolutePath()) && originalDir.mkpath(originalDir.absolutePath()) && correctedDir.mkpath(correctedDir.absolutePath()) && albumArtDir.mkpath(albumArtDir.absolutePath());
}

bool ViProject::removeTempStructure()
{
	QDir dir(mTempPath);
	return dir.removeRecursively();
}

void ViProject::setProjectName(QString name)
{
	mProperties.setProjectName(name);
}

QString ViProject::projectName()
{
	return mProperties.projectName();
}

ViVersion ViProject::createdVersion()
{
	return mProperties.createdVersion();
}

ViVersion ViProject::editedVersion()
{
	return mProperties.editedVersion();
}

ViFileSongInfoList ViProject::songs()
{
	ViFileSongInfoList list = mSongs.songInfos();
	for(int i = 0; i < list.size(); ++i)
	{
		if(list[i].originalFilePath() != "")
		{
			list[i].setOriginalFilePath(mOriginalPath + QDir::separator() + list[i].originalFilePath());
		}
		if(list[i].correctedFilePath() != "")
		{
			list[i].setCorrectedFilePath(mCorrectedPath + QDir::separator() + list[i].correctedFilePath());
		}
		if(list[i].albumArtPath() != "")
		{
			list[i].setAlbumArtPath(mAlbumArtPath + QDir::separator() + list[i].albumArtPath());
		}
	}
	return list;
}

void ViProject::addSong(ViFileSongInfo info)
{
	QFileInfo originalInfo(info.originalFilePath());
	info.setOriginalFilePath(originalInfo.fileName());

	QFileInfo correctedInfo(info.correctedFilePath());
	info.setCorrectedFilePath(correctedInfo.fileName());

	if(info.albumArtPath() != "")
	{
		QFile file(info.albumArtPath());
		QFileInfo imageInfo(info.albumArtPath());
		QString albumArt = originalInfo.baseName() + "." + imageInfo.suffix();
		file.copy(mAlbumArtPath + QDir::separator() + albumArt);
		info.setAlbumArtPath(albumArt);
	}

	mSongs.addSongInfo(info);
}

QString ViProject::nextOriginalSongNumber()
{
	return nextSongNumber(mOriginalPath);
}

QString ViProject::nextCorrectedSongNumber()
{
	return nextSongNumber(mCorrectedPath);
}

QString ViProject::originalPath()
{
	return mOriginalPath;
}

QString ViProject::correctedPath()
{
	return mCorrectedPath;
}

QString ViProject::albumArtPath()
{
	return mAlbumArtPath;
}

QString ViProject::nextSongNumber(QString path)
{
	QDir dir(path);
	QFileInfoList files = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);

	QStringList names;
	for(int i = 0; i < files.size(); ++i)
	{
		names.append(files[i].baseName());
	}

	int number = 1;
	QString name = padName(number);
	while(names.contains(name))
	{
		++number;
		name = padName(number);
	}
	return name;
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
