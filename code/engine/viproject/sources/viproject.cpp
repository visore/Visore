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
    clearObjects();
    clearFiles();
}

/*******************************************************************************************************************

	OBJECTS

*******************************************************************************************************************/

int ViProject::objectCount()
{
    mObjectsMutex.lock();
    int size = mObjects.size();
            mObjectsMutex.unlock();
    return size;
    //return mObjects.size();
}

ViAudioObjectQueue ViProject::objects()
{
    QMutexLocker locker(&mObjectsMutex);
	return mObjects;
}

ViAudioObjectPointer ViProject::object(int index)
{
	if(index >= objectCount())
	{
		return ViAudioObject::createNull();
	}
    QMutexLocker locker(&mObjectsMutex);
	return mObjects[index];
}	

ViAudioObjectPointer ViProject::object(int side, int track)
{
    QMutexLocker locker(&mObjectsMutex);
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
    mObjectsMutex.lock();
	mObjects.enqueue(object);
    mObjectsMutex.unlock();
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
    for(int i = 0; i < objectCount(); ++i)
	{
        mObjectsMutex.lock();
		max = qMax(mObjects[i]->sideNumber(), max);
        mObjectsMutex.unlock();
	}
	return max;
}

int ViProject::trackCount()
{
    return objectCount();
}

int ViProject::trackCount(int side)
{
	int count = 0;
    for(int i = 0; i < objectCount(); ++i)
	{
        mObjectsMutex.lock();
		count += (mObjects[i]->sideNumber() == side);
        mObjectsMutex.unlock();
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

    CLEAR

*******************************************************************************************************************/

void ViProject::clear()
{
    LOG("Clearing project.");
    clearObjects();
    clearFiles();
    emit cleared();
}

void ViProject::clearObjects()
{
    QMutexLocker locker(&mObjectsMutex);
    mObjects.clear();
}

void ViProject::clearFiles()
{
    removeStructure();
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
	mPaths[ViProject::CoverData] = mPaths[ViProject::Data] + "covers" + QDir::separator();

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
    result &= createDirectory(ViProject::CoverData, side);
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
    for(int i = 0; i < objectCount(); ++i)
	{
        mObjectsMutex.lock();
		result.append(mObjects[i]->fileName(track, side));
        mObjectsMutex.unlock();
	}
	return result;
}

void ViProject::moveToProject()
{
	ViAudioObjectPointer object;
    for(int i = 0; i < objectCount(); ++i)
	{
        mObjectsMutex.lock();
		object = mObjects[i];
        mObjectsMutex.unlock();
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
            QString newPath = path(ViProject::CoverData, object->sideNumber()) + object->fileName() + info.imageExtension(".");
            QFile::rename(info.imagePath(), newPath);
            info.changeImagePath(info.imagePath(), newPath);
            object->setSongInfo(info);
		}
	}
}

void ViProject::moveToProject(ViAudioObjectPointer object, ViAudioObject::Type type)
{	
	QString oldPath = object->filePath(type);
    if(!oldPath.startsWith(mPaths[ViProject::Root]))
    {
        QString newPath = path(type, object->sideNumber()) + object->fileName();
        ViAudioCodec *codec = object->format(type).codec();
        if(codec != NULL)
        {
            newPath += codec->extension(".");
        }
        QFile::rename(oldPath, newPath);
        object->setFilePath(type, newPath);
    }
}

/*******************************************************************************************************************

	SAVE & LOAD INFORMATION

*******************************************************************************************************************/

bool ViProject::saveProperties()
{
    ViElement root("properties");
    root.addChild("projectName", mProjectName);

    ViElement created("created");
    ViElement createdVersion("version");
    createdVersion.addChild("major", QString::number(mCreatedVersion.major()));
    createdVersion.addChild("minor", QString::number(mCreatedVersion.minor()));
    createdVersion.addChild("patch", QString::number(mCreatedVersion.patch()));
	created.addChild(createdVersion);
	root.addChild(created);

    ViElement edited("edited");
    ViElement editedVersion("version");
    editedVersion.addChild("major", QString::number(mEditedVersion.major()));
    editedVersion.addChild("minor", QString::number(mEditedVersion.minor()));
    editedVersion.addChild("patch", QString::number(mEditedVersion.patch()));
	edited.addChild(editedVersion);
	root.addChild(edited);

	return root.saveToFile(path(ViProject::Properties));
}

bool ViProject::saveTracks()
{
	ViElement root("sides");
	bool save = true;

	QMap<int, ViElement> sides;
	ViAudioObjectPointer object;
	ViSongInfo info;

    for(int i = 0; i < objectCount(); ++i)
	{
        mObjectsMutex.lock();
		object = mObjects[i];
        mObjectsMutex.unlock();
		info = object->songInfo();
		if(!sides.contains(object->sideNumber()))
		{
			ViElement side("side");
			side.addAttribute("id", object->sideNumber());
			sides[object->sideNumber()] = side;
		}
		
		ViElement track("track");
		track.addAttribute("id", object->trackNumber());
		track.addChild("artist", info.artistName());
		track.addChild("title", info.songTitle());

		ViElement data("data");
        data.addChild("cover", relativePath(info.imagePath()));
		data.addChild("target", relativePath(object->targetFilePath()));
		data.addChild("corrupted", relativePath(object->corruptedFilePath()));
		data.addChild("corrected", relativePath(object->correctedFilePath()));
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

    mProjectName = root.child("projectName").toString();
	
    ViElement created = root.child("created");
    mCreatedVersion = ViVersion(created.child("major").toInt(), created.child("minor").toInt(), created.child("patch").toInt());

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
			info.setArtistName(tracks[i].child("artist").toString());
			info.setSongTitle(tracks[i].child("title").toString());
			ViElement data = tracks[i].child("data");
			object->setTargetFilePath(absolutePath(data.child("target").toString()));
			object->setCorruptedFilePath(absolutePath(data.child("corrupted").toString()));
			object->setCorrectedFilePath(absolutePath(data.child("corrected").toString()));
			info.setImagePath(absolutePath(data.child("cover").toString()));
			object->setSongInfo(info);
			object->setSideNumber(sides[j].attribute("id").toInt());
			object->setTrackNumber(tracks[i].attribute("id").toInt());
            mObjectsMutex.lock();
			mObjects.append(object);
            mObjectsMutex.unlock();
			QObject::connect(object.data(), SIGNAL(encoded()), this, SLOT(save()), Qt::UniqueConnection);
		}
	}
	return true;
}

bool ViProject::loadCorrelations()
{

}
