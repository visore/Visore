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
	mFinished = true;

	setFilePath(filePath);
	setProjectName("");
	mCreatedVersion = ViManager::version();
	mEditedVersion = ViManager::version();

	createStructure();
}

ViProject::ViProject(QString projectName, QString filePath, int sides)
	: QObject(), ViId()
{
	mFinished = true;

	setFilePath(filePath);
	setProjectName(projectName);
	mCreatedVersion = ViManager::version();
	mEditedVersion = ViManager::version();

	createStructure();
}

ViProject::ViProject(const ViProject &other)
{
	mFinished = true;
	mPaths = other.mPaths;
	mFiles = other.mFiles;
	mObjects = other.mObjects;
	mProjectName = other.mProjectName;
	mCreatedVersion = other.mCreatedVersion;
	mEditedVersion = other.mEditedVersion;

	setFilePath(other.filePath());
	createStructure();
}

ViProject::~ViProject()
{
	removeStructure();
	mObjects.clear();
}

/*******************************************************************************************************************

	OBJECTS

*******************************************************************************************************************/

int ViProject::objectCount()
{
	return mObjects.size();
}

ViAudioObjectQueue ViProject::objects()
{
	return mObjects;
}

ViAudioObjectPointer ViProject::object(int index)
{
	if(index >= objectCount())
	{
		return ViAudioObject::createNull();
	}
	return mObjects[index];
}	

ViAudioObjectPointer ViProject::object(int side, int track)
{
	for(int i = 0; i < mObjects.size(); ++i)
	{
		if(mObjects[i]->sideNumber() == side && mObjects[i]->trackNumber() == track)
		{
			return mObjects[i];
		}
	}
	return ViAudioObject::createNull();
}

void ViProject::add(ViAudioObjectPointer object)
{
	QObject::connect(object.data(), SIGNAL(encoded()), this, SLOT(save()), Qt::DirectConnection);
	createDirectory(object->sideNumber());
	mObjects.enqueue(object);
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

QString ViProject::filePath() const
{
	return mArchive.filePath();
}

QString ViProject::fileName() const
{
	QFileInfo info(filePath());
	return info.fileName();
}

bool ViProject::isFinished()
{
	return mFinished;
}

/*******************************************************************************************************************

	SIDES & TRACKS

*******************************************************************************************************************/

int ViProject::sideCount()
{
	int max = 0;
	for(int i = 0; i < mObjects.size(); ++i)
	{
		max = qMax(mObjects[i]->sideNumber(), max);
	}
	return max;
}

int ViProject::trackCount()
{
	return mObjects.size();
}

int ViProject::trackCount(int side)
{
	int count = 0;
	for(int i = 0; i < mObjects.size(); ++i)
	{
		count += (mObjects[i]->sideNumber() == side);
	}
	return count;
}

/*******************************************************************************************************************

	PROPERTIES

*******************************************************************************************************************/

void ViProject::setProjectName(QString name)
{
	mProjectName = name;
}

QString ViProject::projectName()
{
	return mProjectName;
}

ViVersion ViProject::createdVersion()
{
	return mCreatedVersion;
}

ViVersion ViProject::editedVersion()
{
	return mEditedVersion;
}

/*******************************************************************************************************************

	LOAD & SAVE

*******************************************************************************************************************/

bool ViProject::load(bool minimal)
{
	LOG("Loading project.");
	setFinished(false);

	QObject::connect(&mArchive, SIGNAL(decompressed()), this, SLOT(loadAll()), Qt::UniqueConnection);

	if(minimal)
	{
		return mArchive.decompressData(path(ViProject::Root), mArchive.fileList(".vml"));
	}
	else
	{
		return mArchive.decompressData(path(ViProject::Root));
	}
}

void ViProject::save()
{
	LOG("Saving project.");
	setFinished(false);

	QObject::connect(&mArchive, SIGNAL(compressed()), this, SLOT(setFinished()), Qt::UniqueConnection);
	QObject::connect(&mArchive, SIGNAL(compressed()), this, SIGNAL(saved()), Qt::UniqueConnection);

	moveToProject();
	saveAll();
	mArchive.compressData(path(ViProject::Root));
}

bool ViProject::loadAll()
{
	if(mArchive.error() != ViArchive::NoError)
	{
		LOG("Archive could not be fully extracted: " + mArchive.errorString());
		return false;
	}
	bool success = loadProperties() & loadTracks();
	emit loaded();
	setFinished(true);
	return success;
}

void ViProject::saveAll()
{
	saveProperties();
	saveTracks();
}

void ViProject::setFinished(bool finish)
{
	mFinished = finish;
	if(mFinished)
	{
		emit finished();
	}
}

/*******************************************************************************************************************

	DIRECTORIES & FILES

*******************************************************************************************************************/

bool ViProject::createStructure()
{
	bool success = true;

	//Declare paths
	mPaths[ViProject::Root] = ViManager::tempPath() + "projects" + QDir::separator() + id() + QDir::separator();

	mPaths[ViProject::Info] = mPaths[ViProject::Root] + "info" + QDir::separator();
	mPaths[ViProject::GeneralInfo] = mPaths[ViProject::Info] + "general" + QDir::separator();
	mPaths[ViProject::TrackInfo] = mPaths[ViProject::Info] + "tracks" + QDir::separator();
	mPaths[ViProject::CorrectionInfo] = mPaths[ViProject::Info] + "correction" + QDir::separator();
	
	mPaths[ViProject::Data] = mPaths[ViProject::Root] + "data" + QDir::separator();
	mPaths[ViProject::TargetData] = mPaths[ViProject::Data] + "target" + QDir::separator();
	mPaths[ViProject::CorruptedData] = mPaths[ViProject::Data] + "corrupted" + QDir::separator();
	mPaths[ViProject::CorrectedData] = mPaths[ViProject::Data] + "corrected" + QDir::separator();
	mPaths[ViProject::CoverData] = mPaths[ViProject::Data] + "cover" + QDir::separator();

	//Create paths
	for(QMap<ViProject::Directory, QString>::iterator iterator = mPaths.begin(); iterator != mPaths.end(); ++iterator)
	{
		success &= createDirectory(iterator.value());
	}

	//Declare files
	mFiles[ViProject::Properties] = mPaths[ViProject::GeneralInfo] + "properties.vml";	
	mFiles[ViProject::Tracks] = mPaths[ViProject::TrackInfo] + "tracks.vml";	

	//Create files
	for(QMap<ViProject::File, QString>::iterator iterator = mFiles.begin(); iterator != mFiles.end(); ++iterator)
	{
		success &= createFile(iterator.value());
	}

	return success;
}

bool ViProject::removeStructure()
{
	QDir dir(path(ViProject::Root));
	return dir.removeRecursively();
}

bool ViProject::createDirectory(QString directoryPath)
{
	QDir dir;
	return dir.mkpath(directoryPath);
}

bool ViProject::createDirectory(int side)
{
	bool result = true;
	result &= createDirectory(ViProject::TargetData, side);
	result &= createDirectory(ViProject::CorruptedData, side);
	result &= createDirectory(ViProject::CorrectedData, side);
	return result;
}

bool ViProject::createDirectory(ViProject::Directory directory, int side)
{
	return createDirectory(path(directory) + "side" + QString::number(side));
}

bool ViProject::removeDirectory(QString directoryPath)
{
	QDir dir(directoryPath);
	return dir.removeRecursively();
}

bool ViProject::createFile(QString filePath)
{
	QFile file(filePath);
	if(!file.exists())
	{
		if(file.open(QIODevice::ReadWrite))
		{
			file.close();
			return true;
		}
		return false;
	}
	return true;
}

bool ViProject::removeFile(QString filePath)
{
	return QFile::remove(filePath);
}

QString ViProject::relativePath(QString path)
{
	return path.replace(mPaths[ViProject::Root], "");
}

QString ViProject::absolutePath(QString path)
{
	if(path == "" || path.startsWith(mPaths[ViProject::Root]))
	{
		return path;
	}
	if(path.startsWith("/"))
	{
		path.remove(0, 1);
	}
	return mPaths[ViProject::Root] + path;
}

QString ViProject::path(ViProject::Directory directory)
{
	if(!mPaths.contains(directory))
	{
		return "";
	}
	return mPaths[directory];
}

QString ViProject::path(ViProject::File file)
{
	if(!mFiles.contains(file))
	{
		return "";
	}
	return mFiles[file];
}

QString ViProject::path(ViProject::Directory directory, int side)
{
	return path(directory) + "side" + QString::number(side) + QDir::separator();
}

QString ViProject::path(ViAudioObject::Type type, int side)
{
	if(type == ViAudioObject::Target)
	{
		return path(ViProject::TargetData, side);
	}
	else if(type == ViAudioObject::Corrupted)
	{
		return path(ViProject::CorruptedData, side);
	}
	else if(type == ViAudioObject::Corrected)
	{
		return path(ViProject::CorrectedData, side);
	}
	return "";
}

QStringList ViProject::fileNames(bool track, bool side)
{
	QStringList result;
	for(int i = 0; i < mObjects.size(); ++i)
	{
		result.append(mObjects[i]->fileName(track, side));
	}
	return result;
}

void ViProject::moveToProject()
{
	ViAudioObjectPointer object;
	for(int i = 0; i < mObjects.size(); ++i)
	{
		object = mObjects[i];
		if(object->hasFile(ViAudioObject::Target))
		{
			moveToProject(object, ViAudioObject::Target);
		}
		if(object->hasFile(ViAudioObject::Corrupted))
		{
			moveToProject(object, ViAudioObject::Corrupted);
		}
		if(object->hasFile(ViAudioObject::Corrected))
		{
			moveToProject(object, ViAudioObject::Corrected);
		}
		ViSongInfo info = object->songInfo();
		if(info.imagePath() != "")
		{
			QFile::rename(info.imagePath(), path(ViProject::CoverData) + object->fileName() + info.imageExtension("."));
		}
	}
}

void ViProject::moveToProject(ViAudioObjectPointer object, ViAudioObject::Type type)
{	
	QString oldPath = object->filePath(type);
	QString newPath = path(type, object->sideNumber()) + object->fileName();
	ViAudioCodec *codec = object->format(type).codec();
	if(codec != NULL)
	{
		newPath += codec->extension(".");
	}
	QFile::rename(oldPath, newPath);
	object->setFilePath(type, newPath);
}

/*******************************************************************************************************************

	SAVE & LOAD INFORMATION

*******************************************************************************************************************/

bool ViProject::saveProperties()
{
	ViElement root("Properties");
	root.addChild("ProjectName", mProjectName);

	ViElement created("Created");
	ViElement createdVersion("Version");
	createdVersion.addChild("Major", QString::number(mCreatedVersion.major()));
	createdVersion.addChild("Minor", QString::number(mCreatedVersion.minor()));
	createdVersion.addChild("Patch", QString::number(mCreatedVersion.patch()));
	created.addChild(createdVersion);
	root.addChild(created);

	ViElement edited("Edited");
	ViElement editedVersion("Version");
	editedVersion.addChild("Major", QString::number(mEditedVersion.major()));
	editedVersion.addChild("Minor", QString::number(mEditedVersion.minor()));
	editedVersion.addChild("Patch", QString::number(mEditedVersion.patch()));
	edited.addChild(editedVersion);
	root.addChild(edited);

	return root.saveToFile(path(ViProject::Properties));
}

bool ViProject::saveTracks()
{
	ViElement root("Tracks");
	bool save = true;

	QMap<int, ViElement> sides;
	ViAudioObjectPointer object;
	ViSongInfo info;
	
	for(int i = 0; i < mObjects.size(); ++i)
	{
		object = mObjects[i];
		info = object->songInfo();
		if(!sides.contains(object->sideNumber()))
		{
			ViElement side("Side");
			side.addAttribute("id", object->sideNumber());
			sides[object->sideNumber()] = side;
		}
		
		ViElement track("Track");
		track.addAttribute("id", object->trackNumber());
		track.addChild("Artist", info.artistName());
		track.addChild("Title", info.songTitle());

		ViElement data("Data");
		data.addChild("AlbumArt", info.imagePath());
		data.addChild("Target", relativePath(object->targetFilePath()));
		data.addChild("Corrupted", relativePath(object->corruptedFilePath()));
		data.addChild("Corrected", relativePath(object->correctedFilePath()));
		track.addChild(data);

		sides[object->sideNumber()].addChild(track);
	}

	QList<int> keys = sides.keys();
	for(int i = 0; i < keys.size(); ++i)
	{
		root.addChild(sides[keys[i]]);
	}

	return root.saveToFile(path(ViProject::Tracks));
}

bool ViProject::saveCorrelations()
{

}

bool ViProject::loadProperties()
{
	ViElement root;
	if(!root.loadFromFile(path(ViProject::Properties)))
	{
		return false;
	}

	mProjectName = root.child("ProjectName").toString();
	
	ViElement created = root.child("Created");
	mCreatedVersion = ViVersion(created.child("Major").toInt(), created.child("Minor").toInt(), created.child("Patch").toInt());

	return true;
}

bool ViProject::loadTracks()
{
	ViElement root;
	if(!root.loadFromFile(path(ViProject::Tracks)))
	{
		return false;
	}

	ViElementList sides = root.children();
	for(int j = 0; j < sides.size(); ++j)
	{
		ViElementList tracks = sides[j].children();
		for(int i = 0; i < tracks.size(); ++i)
		{
			ViAudioObjectPointer object = ViAudioObject::create();
			ViSongInfo info;
			info.setArtistName(tracks[i].child("Artist").toString());
			info.setSongTitle(tracks[i].child("Title").toString());
			ViElement data = tracks[i].child("Data");
			object->setTargetFilePath(absolutePath(data.child("Target").toString()));
			object->setCorruptedFilePath(absolutePath(data.child("Corrupted").toString()));
			object->setCorrectedFilePath(absolutePath(data.child("Corrected").toString()));
			info.setImagePath(absolutePath(data.child("AlbumArt").toString()));
			object->setSongInfo(info);
			object->setSideNumber(sides[j].attribute("id").toInt());
			object->setTrackNumber(tracks[i].attribute("id").toInt());
			mObjects.append(object);
			QObject::connect(object.data(), SIGNAL(encoded()), this, SLOT(save()), Qt::UniqueConnection);
		}
	}
	return true;
}

bool ViProject::loadCorrelations()
{

}
