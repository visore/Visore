#include "viproject.h"
#include "viaudiocodec.h"
#include <vilogger.h>
#include <viprocessor.h>
#include <viglobalcorrelation.h>
#include <QDateTime>

#define RECORDING_NAME_LENGTH 2

/*******************************************************************************************************************

            CONSTRUCTORS & DESTRUCTORS

*******************************************************************************************************************/

ViProject::ViProject(QString filePath)
    : QObject(), ViId()
{
    mFinished = true;
    mCurrentCorrectionId = "";

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
    mCurrentCorrectionId = "";

    setFilePath(filePath);
    setProjectName(projectName);
    mCreatedVersion = ViManager::version();
    mEditedVersion = ViManager::version();

    createStructure();
}

ViProject::ViProject(const ViProject &other)
{
    mFinished = true;

    mCurrentCorrectionId = other.mCurrentCorrectionId;

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
	clear();
}

/*******************************************************************************************************************

    OBJECTS

*******************************************************************************************************************/

int ViProject::objectIndex(const QString &id)
{
	for(int i = 0; i < mObjects.size(); ++i)
	{
		if(id == mObjects[i]->id())
		{
			return i;
		}
	}
	return -1;
}

int ViProject::objectIndex(ViAudioObjectPointer object)
{
	return objectIndex(object->id());
}

int ViProject::objectCount()
{
    return mObjects.size();
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
    QObject::connect(object.data(), SIGNAL(changed()), this, SLOT(save()), Qt::DirectConnection);
	QObject::connect(object.data(), SIGNAL(started()), this, SLOT(setBusy()), Qt::UniqueConnection);
	QObject::connect(object.data(), SIGNAL(finished()), this, SLOT(setFinished()), Qt::UniqueConnection);

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

/*******************************************************************************************************************

    CORRELATIONS

*******************************************************************************************************************/


void ViProject::bestCorrection(const QString &fileId, const ViCorrelationGroup &group1, const ViCorrelationGroup &group2, QHash<QString, ViCorrelation> &bestCorrelation, QHash<QString, QString> &bestId, QHash<QString, qreal> &bestImprovement)
{
	QStringList correlators = group1.correlators();
	QString correlator;
	for(int i = 0; i < correlators.size(); ++i)
	{
		correlator = correlators[i];
		if(correlator != "")
		{
			qreal improvement = (group1.correlation(correlator).mean() - group2.correlation(correlator).mean()) / (1 - group2.correlation(correlator).mean());
			if(!bestImprovement.contains(correlator) || improvement >= bestImprovement[correlator])
			{
				bestImprovement[correlator] = improvement;
				bestId[correlator] = fileId;
				bestCorrelation[correlator] = group1.correlation(correlator);
			}
		}
	}
}

ViCorrelation ViProject::bestCorrelation(QString correlator, int songIndex)
{
	if(songIndex < 0)
	{
		return mBestCorrelation.correlation(correlator);
	}
	else if(mBestTrackCorrelation.contains(songIndex))
	{
		return mBestTrackCorrelation[songIndex].correlation(correlator);
	}
	return ViCorrelation();
}

qreal ViProject::bestImprovement(QString correlator, int songIndex)
{
	if(songIndex < 0)
	{
		return mBestImprovement[correlator];
	}
	else if(mBestTrackImprovement.contains(songIndex))
	{
		return mBestTrackImprovement[songIndex][correlator];
	}
	return 0;
}

QString ViProject::bestCorrectionId(QString correlator, int songIndex)
{
	if(songIndex < 0)
	{
		return mBestCorrectionId[correlator];
	}
	else if(mBestTrackCorrectionId.contains(songIndex))
	{
		return mBestTrackCorrectionId[songIndex][correlator];
	}
	return "";
}

QString ViProject::currentCorrectionId()
{
    return mCurrentCorrectionId;
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
	while(!isFinished());
    LOG("Clearing project.");
    clearObjects();
    clearFiles();
    clearCorrections();
    clearCorrelations();
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

void ViProject::clearCorrections()
{
    mCurrentCorrectionId = "";
}

void ViProject::clearCorrelations()
{
    mBestCorrelation.clear();
    mBestCorrectionId.clear();
    mBestImprovement.clear();
}

/*******************************************************************************************************************

    LOAD & SAVE

*******************************************************************************************************************/

bool ViProject::load(bool minimal)
{
    LOG("Loading project.");
	setBusy();

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
	setBusy();

    QObject::connect(&mArchive, SIGNAL(compressed()), this, SLOT(setFinished()), Qt::UniqueConnection);
    QObject::connect(&mArchive, SIGNAL(compressed()), this, SIGNAL(saved()), Qt::UniqueConnection);

	moveToProject();
	cleanProject();
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
    bool success = loadProperties() & loadTracks() & loadCorrections();
    emit loaded();
	setFinished();
    return success;
}

void ViProject::saveAll()
{
	saveProperties();
	saveTracks();
	saveCorrections();
}

void ViProject::setFinished()
{
	QMutexLocker locker(&mFinishedMutex);
	mFinished = true;
	for(int i = 0; i < mObjects.size(); ++i)
	{
		if(mObjects[i]->isBusy())
		{
			mFinished = false;
			break;
		}
	}
	if(mFinished)
	{
		locker.unlock();
		emit finished();
	}
}

void ViProject::setBusy()
{
	QMutexLocker locker(&mFinishedMutex);
	mFinished = false;
}

bool ViProject::isFinished()
{
	QMutexLocker locker(&mFinishedMutex);
	return mFinished;
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

QString ViProject::path(ViAudio::Type type, int side)
{
    if(type == ViAudio::Target)
    {
        return path(ViProject::TargetData, side);
    }
    else if(type == ViAudio::Corrupted)
    {
        return path(ViProject::CorruptedData, side);
    }
    else if(type == ViAudio::Corrected)
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

void ViProject::cleanProject()
{
	QList<ViProject::Directory> types = {ViProject::TargetData, ViProject::CorruptedData, ViProject::CorrectedData, ViProject::CoverData};
	QList<ViAudio::Type> audioTypes = {ViAudio::Target, ViAudio::Corrupted, ViAudio::Corrected};
	QDir dir;
	QFileInfoList files;
	bool found;
	int sides = sideCount() + 1;
	for(int i = 1; i < sides; ++i)
	{
		for(int t = 0; t < types.size(); ++t)
		{
			dir.setPath(path(types[t], i));
			files = dir.entryInfoList(QDir::Files);
			for(int j = 0; j < files.size(); ++j)
			{
				found = false;
				for(int k = 0; k < objectCount(); ++k)
				{
					if(types[t] == ViProject::CoverData)
					{
						if(files[j].absoluteFilePath() == mObjects[k]->metadata().cover()) found = true;
					}
					else
					{
						if(files[j].absoluteFilePath() == mObjects[k]->filePath(audioTypes[t])) found = true;
					}
				}
				if(!found) QFile::remove(files[j].absoluteFilePath());
			}
		}
	}
}

void ViProject::moveToProject()
{
    ViAudioObjectPointer object;
    for(int i = 0; i < objectCount(); ++i)
    {
        mObjectsMutex.lock();
        object = mObjects[i];
        mObjectsMutex.unlock();
        if(object->hasFile(ViAudio::Target))
        {
            moveToProject(object, ViAudio::Target);
        }
        if(object->hasFile(ViAudio::Corrupted))
        {
            moveToProject(object, ViAudio::Corrupted);
        }
        if(object->hasFile(ViAudio::Corrected))
        {
            moveToProject(object, ViAudio::Corrected);
		}

		ViMetadata &metadata = object->metadata();
		if(metadata.hasCover())
		{
			bool deleteOld = false;
			if(metadata.cover().startsWith(ViManager::tempCoverPath()))
			{
				deleteOld = true;
			}
			metadata.copyCover(path(ViProject::CoverData, object->sideNumber()) + object->fileName() + "." + metadata.coverFormat(), deleteOld);
		}
	}
}

void ViProject::moveToProject(ViAudioObjectPointer object, ViAudio::Type type)
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
		QFile::remove(newPath); // Make sure the old file is removed first
		QFile::rename(oldPath, newPath);
        object->setFilePath(type, newPath);
    }
}

/*******************************************************************************************************************

    CORRECTIONS

*******************************************************************************************************************/

QString ViProject::nextCorrectionId()
{
    QDir dir(path(ViProject::CorrectionInfo));
    QFileInfoList list = dir.entryInfoList(QDir::Files);
	int id = 0;
	if(!list.isEmpty())
	{
		id = list.last().baseName().replace("correction", "").toInt();
	}
    ++id;
    mCurrentCorrectionId = QString::number(id).rightJustified(6, '0');
    return mCurrentCorrectionId;
}

QString ViProject::correctionId(QString path)
{
    if(path == "")
    {
        nextCorrectionId();
    }
    return QFileInfo(path).baseName().replace("correction", "");
}

QString ViProject::correctionPath(QString id)
{
    QString result = path(ViProject::CorrectionInfo) + "correction";
    if(id == "")
	{
        result += nextCorrectionId();
    }
    else
	{
        result += id.rightJustified(6, '0');
    }
    return result + ".vml";
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
    QMap<int, ViElement> sides;
    ViAudioObjectPointer object;
	ViMetadata metadata;

    for(int i = 0; i < objectCount(); ++i)
    {
        mObjectsMutex.lock();
        object = mObjects[i];
        mObjectsMutex.unlock();
		metadata = object->metadata();
        if(!sides.contains(object->sideNumber()))
        {
            ViElement side("side");
			side.addAttribute("number", object->sideNumber());
            sides[object->sideNumber()] = side;
        }

        ViElement track("track");
		track.addAttribute("number", object->trackNumber());
		track.addChild("id", object->id());
		track.addChild("artist", metadata.artist(false));
		track.addChild("title", metadata.title(false));
		track.addChild("album", metadata.album(false));

        ViElement data("data");
		data.addChild("cover", relativePath(metadata.cover()));
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

bool ViProject::saveCorrections()
{
    bool save = false;
    ViGlobalCorrelation globalCorrelation;
    ViElement root("corrections");

	ViAudioObjectPointer theObject;
	ViElement corrector;
    for(int i = 0; i < objectCount(); ++i)
    {
		theObject = object(i);
		if(theObject->hasCorrector())
        {
			if(!save)
			{
				corrector = theObject->corrector()->exportData();
				save = true;
			}

            ViElement correction("correction");

			ViElement track("track");
			track.addChild("id", theObject->id());
			track.addChild("name", theObject->fileName());
			correction.addChild(track);

			ViCorrelationGroups correlationObjects = theObject->correlations();
			globalCorrelation.add(correlationObjects);

			ViElement correlations("correlations");
            for(int j = 0; j < correlationObjects.size(); ++j)
            {
				correlations.addChild(correlationObjects[j].exportData());
			}
            correction.addChild(correlations);
			root.addChild(correction);
        }
    }

	bool success = false;
	if(save && !globalCorrelation.correlations().isEmpty())
    {
		root.addChild(corrector);
		root.prependChild(globalCorrelation.exportData());
		QDateTime dateTime = QDateTime::currentDateTime();
		root.prependChild("time", dateTime.time().toString("HH:mm:ss"));
		root.prependChild("date", dateTime.date().toString("dd-MM-yyyy"));
        success = root.saveToFile(correctionPath(mCurrentCorrectionId));
    }
	loadCorrections(); //Ensures that the current correlation is included
    return success;
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
			ViMetadata metadata;
			metadata.setArtist(tracks[i].child("artist").toString());
			metadata.setTitle(tracks[i].child("title").toString());
			metadata.setAlbum(tracks[i].child("album").toString());

            ViElement data = tracks[i].child("data");
            object->setTargetFilePath(absolutePath(data.child("target").toString()));
            object->setCorruptedFilePath(absolutePath(data.child("corrupted").toString()));
            object->setCorrectedFilePath(absolutePath(data.child("corrected").toString()));

			QString coverPath = absolutePath(data.child("cover").toString());
			QFile file(coverPath);
			if(!file.exists()) coverPath = "";
			metadata.setCover(coverPath);

			object->setId(tracks[i].child("id").toString());
			object->setMetadata(metadata);
			object->setSideNumber(sides[j].attribute("number").toInt());
			object->setTrackNumber(tracks[i].attribute("number").toInt());

            mObjectsMutex.lock();
            mObjects.append(object);
            mObjectsMutex.unlock();

            QObject::connect(object.data(), SIGNAL(changed()), this, SLOT(save()), Qt::DirectConnection);
			QObject::connect(object.data(), SIGNAL(started()), this, SLOT(setBusy()), Qt::UniqueConnection);
			QObject::connect(object.data(), SIGNAL(finished()), this, SLOT(setFinished()), Qt::UniqueConnection);
        }
    }
    return true;
}

bool ViProject::loadCorrections()
{
    bool success = true;
    clearCorrelations();

    QFileInfoList files = QDir(path(ViProject::CorrectionInfo)).entryInfoList(QDir::Files);

	QHash<QString, ViCorrelation> bestCorrelation;
	QHash<QString, QString> bestId;
	QHash<QString, qreal> bestImprovement;

	QHash<int, QHash<QString, ViCorrelation>> bestTrackCorrelation;
	QHash<int, QHash<QString, QString>> bestTrackId;
	QHash<int, QHash<QString, qreal>> bestTrackImprovement;

    for(int i = 0; i < files.size(); ++i)
    {
		QString fileId = files[i].baseName().replace("correction", "");
		ViElement root;
        success &= root.loadFromFile(files[i].absoluteFilePath());

		//Global correlation
		ViGlobalCorrelation global;
        success &= global.importData(root);
		ViCorrelationGroup group1 = global.correlation(ViAudio::Target, ViAudio::Corrected);
		ViCorrelationGroup group2 = global.correlation(ViAudio::Target, ViAudio::Corrupted);
		bestCorrection(fileId, group1, group2, bestCorrelation, bestId, bestImprovement);

		//Track correlation
		ViElementList corrections = root.children("correction");
		int index;
		QString id;
		for(int j = 0; j < corrections.size(); ++j)
		{
			index = -1;
			id = corrections[j].child("track").child("id").toString();
			for(int k = 0; k < mObjects.size(); ++k)
			{
				if(id == mObjects[k]->id())
				{
					index = k;
					break;
				}
			}
			if(index >= 0)
			{
				ViElementList correlations = corrections[j].child("correlations").children("correlationgroup");
				if(correlations.size() == 2)
				{
					group1.clear();
					success &= group1.importData(correlations[0]);
					group2.clear();
					success &= group2.importData(correlations[1]);
					bestCorrection(fileId, group1, group2, bestTrackCorrelation[index], bestTrackId[index], bestTrackImprovement[index]);
				}
			}
			else
			{
				success = false;
				LOG("Can't find the object for the specified correlation.", QtCriticalMsg);
			}
		}
    }

    QString key;

	foreach(key, bestCorrelation.keys())
	{
		mBestCorrelation.set(key, bestCorrelation[key]);
		mBestCorrectionId[key] = bestId[key];
		mBestImprovement[key] = bestImprovement[key];
	}

	int trackKey;
	foreach(trackKey, bestTrackCorrelation.keys())
	{
		QHash<QString, ViCorrelation> &correlations = bestTrackCorrelation[trackKey];
		QHash<QString, QString> &ids = bestTrackId[trackKey];
		QHash<QString, qreal> &improvements = bestTrackImprovement[trackKey];
		foreach(key, correlations.keys())
		{
			mBestTrackCorrelation[trackKey].set(key, correlations[key]);
			mBestTrackCorrectionId[trackKey][key] = ids[key];
			mBestTrackImprovement[trackKey][key] = improvements[key];
		}
	}

    return success;
}

