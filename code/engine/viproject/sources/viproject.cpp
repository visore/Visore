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
	mCurrentSide = 0;
	mCurrentTrack = 0;
	mExistingProject = false;

	setFilePath(filePath);
	setSides(1);

	setProjectName("");
	mCreatedVersion = ViManager::version();
	mEditedVersion = ViManager::version();

	QObject::connect(&mEncoder, SIGNAL(finished()), this, SLOT(save()));

	createTempStructure();
}

ViProject::ViProject(QString projectName, QString filePath, int sides)
	: QObject(), ViId()
{
	mCurrentSide = 0;
	mCurrentTrack = 0;
	mExistingProject = false;

	setFilePath(filePath);
	setSides(sides);

	setProjectName(projectName);
	mCreatedVersion = ViManager::version();
	mEditedVersion = ViManager::version();

	QObject::connect(&mEncoder, SIGNAL(finished()), this, SLOT(save()));

	createTempStructure();
}

ViProject::~ViProject()
{
	removeTempStructure();
	mObjects.clear();
}

void ViProject::serialize(ViAudioObjectPointer object, ViAudio::Type type)
{
	QString filePath;
	if(type == ViAudio::TargetType)
	{
		filePath = mPaths["data_target"];
	}
	else if(type == ViAudio::CorruptedType)
	{
		filePath = mPaths["data_corrupted"];
	}
	else if(type == ViAudio::CorrectedType)
	{
		filePath = mPaths["data_corrected"];
	}
	filePath = generateFileName(object->songInfo(), filePath, mFormat.codec()->extension());
	object->setFilePath(type, filePath);

	++mCurrentTrack;
	if(mExistingProject)
	{
		if(type == ViAudio::TargetType)
		{
			mObjects[mCurrentSide-1][mCurrentTrack-1]->setTargetBuffer(object->outputBuffer());
			object->addDestructRule(ViAudio::TargetType, false);
		}
		else if(type == ViAudio::CorruptedType)
		{
			mObjects[mCurrentSide-1][mCurrentTrack-1]->setCorruptedBuffer(object->outputBuffer());
			object->addDestructRule(ViAudio::CorruptedType, false);
		}
		else if(type == ViAudio::CorrectedType)
		{
			mObjects[mCurrentSide-1][mCurrentTrack-1]->setCorrectedBuffer(object->outputBuffer());
			object->addDestructRule(ViAudio::CorrectedType, false);
		}
	}
	else
	{
		mObjects[mCurrentSide-1].append(object);
	}

	if(object->songInfo().hasImage())
	{
		QFileInfo info(object->songInfo().imagePath());
		QString albumArt = generateFileName(object->songInfo(), mPaths["data_albumart"], info.suffix());
		QFile file(object->songInfo().imagePath());
		if(file.exists())
		{
			file.copy(albumArt);
		}
		object->songInfo().changeImagePath(object->songInfo().imagePath(), albumArt);
	}
	mEncoder.encode(object->outputBuffer(), filePath, mFormat, 0, object->songInfo());
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

/*******************************************************************************************************************

	SIDES

*******************************************************************************************************************/

void ViProject::setSides(int sides)
{
	mSides = sides;
	mCurrentSide = 0;
	nextSide();
	mObjects.clear();
	for(int i = 0; i < mSides; ++i)
	{
		mObjects.append(QList<ViAudioObjectPointer>());
	}
	removeSideStructure();
	createSideStructure();
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

	QObject::connect(&mArchive, SIGNAL(decompressFinished()), this, SLOT(loadAll()), Qt::UniqueConnection);

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
	LOG("Saving project.");
	saveAll();
	mArchive.compressData(mPaths["root"]);
	clearObjects();
}

bool ViProject::loadAll()
{
	if(mArchive.error() != ViArchive::NoError)
	{
		LOG("Archive could not be fully extracted: " + mArchive.errorString());
		return false;
	}
	removeSideStructure();
	mExistingProject = true;
	return loadProperties() & loadTracks();
}

void ViProject::saveAll()
{
	saveProperties();
	saveTracks();
}

bool ViProject::createTempStructure()
{
	//Declare paths
	mPaths["root"] = ViManager::tempPath() + QDir::separator() + "projects" + QDir::separator() + id() + QDir::separator();

	mPaths["info"] = mPaths["root"] + "info" + QDir::separator();
	mPaths["info_general"] = mPaths["info"] + "general" + QDir::separator();
	mPaths["info_tracks"] = mPaths["info"] + "tracks" + QDir::separator();
	mPaths["info_correlations"] = mPaths["info"] + "correlations" + QDir::separator();
	
	mPaths["data"] = mPaths["root"] + "data" + QDir::separator();
	mPaths["data_target"] = mPaths["data"] + "target" + QDir::separator();
	mPaths["data_corrupted"] = mPaths["data"] + "corrupted" + QDir::separator();
	mPaths["data_corrected"] = mPaths["data"] + "corrected" + QDir::separator();
	mPaths["data_albumart"] = mPaths["data"] + "albumart" + QDir::separator();

	//Create paths
	bool success = true;
	for(QMap<QString,QString>::iterator iterator = mPaths.begin(); iterator != mPaths.end(); ++iterator)
	{
		QDir dir(iterator.value());
		success &= dir.mkpath(dir.absolutePath());
	}
	success &= createSideStructure();

	mFiles["properties"] = mPaths["info_general"] + "properties.vml";	
	mFiles["tracks"] = mPaths["info_tracks"] + "tracks.vml";	

	return success;
}

bool ViProject::removeTempStructure()
{
	QDir dir(mPaths["root"]);
	return dir.removeRecursively();
}

bool ViProject::createSideStructure()
{
	//Declare paths
	QMap<QString,QString> sides;
	for(int i = 1; i <= mSides; ++i)
	{
		sides["data_target_side" + QString::number(i)] = mPaths["data_target"] + "side" + QString::number(i) + QDir::separator();
		sides["data_corrupted_side" + QString::number(i)] = mPaths["data_corrupted"] + "side" + QString::number(i) + QDir::separator();
		sides["data_corrected_side" + QString::number(i)] = mPaths["data_corrected"] + "side" + QString::number(i) + QDir::separator();
		sides["data_albumart_side" + QString::number(i)] = mPaths["data_albumart"] + "side" + QString::number(i) + QDir::separator();
	}

	//Create paths
	bool success = true;
	for(QMap<QString,QString>::iterator iterator = sides.begin(); iterator != sides.end(); ++iterator)
	{
		QDir dir(iterator.value());
		success &= dir.mkpath(dir.absolutePath());
		mPaths[iterator.key()] = iterator.value();
	}	

	return success;
}

bool ViProject::removeSideStructure()
{
	QStringList paths;
	for(int i = 1; i <= mSides; ++i)
	{
		paths.append(mPaths["data_target_side" + QString::number(i)]);
		paths.append(mPaths["data_corrupted_side" + QString::number(i)]);
		paths.append(mPaths["data_corrected_side" + QString::number(i)]);
		paths.append(mPaths["data_albumart_side" + QString::number(i)]);
	}
	QDir dir;
	bool success = true;
	for(int i = 0; i < paths.size(); ++i)
	{
		if(paths[i] != "")
		{
			dir = QDir(paths[i]);
			if(dir.exists())
			{
				success &= dir.rmdir(dir.absolutePath());
			}
		}
	}
	return success;
}

QString ViProject::generateFileName(ViSongInfo info, QString folder, QString extension)
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
	if(extension != "")
	{
		folder += "." + extension;
	}
	
	return folder;
}

QString ViProject::relativePath(QString path)
{
	return path.replace(mPaths["root"], "");
}

QString ViProject::absolutePath(QString path)
{
	if(path.startsWith(mPaths["root"]))
	{
		return path;
	}
	if(path.startsWith("/"))
	{
		path.remove(0, 1);
	}
	return mPaths["root"] + path;
}

void ViProject::clearObjects()
{
	for(int i = 0; i < mObjects.size(); ++i)
	{
		for(int j = 0; j < mObjects[i].size(); ++j)
		{
			if(!mObjects[i][j]->isUsed())
			{
				mObjects[i][j]->clearBuffers();
			}
		}
	}
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

	return root.saveToFile(mFiles["properties"]);
}

bool ViProject::saveTracks()
{
	ViElement root("Tracks");
	bool save = true;

	for(int j = 0; j < mObjects.size(); ++j)
	{
		ViElement side("Side");
		side.addAttribute("id", (j + 1));
		for(int i = 0; i < mObjects[j].size(); ++i)
		{
			ViElement track("Track");
			track.addChild("Artist", mObjects[j][i]->songInfo().artistName());
			track.addChild("Title", mObjects[j][i]->songInfo().songTitle());

			ViElement data("Data");
			data.addChild("AlbumArt", relativePath(mObjects[j][i]->songInfo().imagePath()));
			data.addChild("Target", relativePath(mObjects[j][i]->targetFilePath()));
			data.addChild("Corrupted", relativePath(mObjects[j][i]->corruptedFilePath()));
			data.addChild("Corrected", relativePath(mObjects[j][i]->correctedFilePath()));
			track.addChild(data);

			side.addChild(track);
		}
		root.addChild(side);
	}

	return root.saveToFile(mFiles["tracks"]);
}

bool ViProject::saveCorrelations()
{

}

bool ViProject::loadProperties()
{
	ViElement root;
	if(!root.loadFromFile(mFiles["properties"]))
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
	if(!root.loadFromFile(mFiles["tracks"]))
	{
		return false;
	}

	ViElementList sides = root.children();
	setSides(sides.size());
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
			mObjects[j].append(object);
		}
	}
	return true;
}

bool ViProject::loadCorrelations()
{

}
