#include "viproject.h"

#define RECORDING_NAME_LENGTH 2

ViProject::ViProject(QString filePath)
	: QObject()
{
	mTempPath = "";
	mDataPath = "";
	mOriginalPath = "";
	mCorrectedPath = "";
	mAlbumArtPath = "";
	setFilePath(filePath);

	mProjectFiles.append(&mProperties);
	mProjectFiles.append(&mSongs);
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
	mAlbumArtPath = mDataPath + QDir::separator() + "albumart";

	for(int i = 0; i < mProjectFiles.size(); ++i)
	{
		mProjectFiles[i]->setDirectory(mTempPath);
	}
	QDir originalDir(mOriginalPath);
	QDir correctedDir(mCorrectedPath);
	QDir albumArtDir(mAlbumArtPath);
	return originalDir.mkpath(originalDir.absolutePath()) && correctedDir.mkpath(correctedDir.absolutePath()) && albumArtDir.mkpath(albumArtDir.absolutePath());
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
	QFileInfo fileInfo;
	QString fileName = "";
	fileInfo.setFile(info.originalFilePath());
	fileName = fileInfo.fileName();
	info.setOriginalFilePath(fileName);

	if(fileName == "")
	{
		fileName = fileInfo.fileName();
	}
	fileInfo.setFile(info.correctedFilePath());
	info.setCorrectedFilePath(fileInfo.fileName());

	if(info.albumArtPath() != "")
	{
		QFile file(info.albumArtPath());
		QString albumArt = mAlbumArtPath + QDir::separator() + fileName;
cout<<info.albumArtPath().toAscii().data()<<"    "<<albumArt.toAscii().data()<<endl;
		file.copy(albumArt);
		fileInfo.setFile(albumArt);
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
