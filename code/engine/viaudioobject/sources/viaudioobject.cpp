#include "viaudioobject.h"
#include <QSet>

/*******************************************************************************************************************

	CONSTRUCTORS & DESTRUCTORS

*******************************************************************************************************************/

ViAudioObject::ViAudioObject(bool autoDestruct)
	: QObject(), ViId()
{
	setAutoDestruct(autoDestruct);
	mIsFinished = false;
	mIsSong = false;

	mInputType = ViAudio::UnknownType;
	mOutputType = ViAudio::UnknownType;

	mTargetBuffer = NULL;
	mCorruptedBuffer = NULL;
	mCorrectedBuffer = NULL;
	mTempBuffer = NULL;

	mTargetFile = "";
	mCorruptedFile = "";
	mCorrectedFile = "";
}

ViAudioObject::~ViAudioObject()
{
	clearBuffers(mDestructType);
}

ViAudioObjectPointer ViAudioObject::create(ViAudioObject *object)
{
	return ViAudioObjectPointer(object);
}

ViAudioObjectPointer ViAudioObject::create(bool autoDestruct)
{
	return ViAudioObjectPointer(new ViAudioObject(autoDestruct));
}

ViAudioObjectPointer ViAudioObject::createNull()
{
	return ViAudioObjectPointer(NULL);
}

/*******************************************************************************************************************

	AUTO DESTRUCT

*******************************************************************************************************************/

void ViAudioObject::setAutoDestruct(bool destruct)
{
	QMutexLocker locker(&mMutex);
	if(destruct)
	{
		mDestructType = ViAudio::AllTypes;
	}
	else
	{
		mDestructType = ViAudio::UnknownType;
	}
}

void ViAudioObject::addDestructRule(ViAudio::Type type, bool destruct)
{
	QMutexLocker locker(&mMutex);
	QSet<ViAudio::Type> set;
	QSet<ViAudio::Type> values;
	values.insert(ViAudio::TargetType);
	values.insert(ViAudio::CorruptedType);
	values.insert(ViAudio::CorrectedType);
	values.insert(ViAudio::TemporaryType);

	//Old Values
	foreach(const ViAudio::Type &value, values)
	{
		if(mDestructType & value)
		{
			set.insert(value);
		}
	}

	//New Values
	foreach(const ViAudio::Type &value, values)
	{
		if(type & value)
		{
			if(destruct)
			{
				set.insert(value);
			}
			else
			{
				set.remove(value);
			}
		}
	}

	if(set.isEmpty())
	{
		mDestructType = ViAudio::UnknownType;
	}
	else
	{
		QList<ViAudio::Type> list = set.toList();
		ViAudio::Type temp = list[0];
		for(int i = 1; i < list.size(); ++i)
		{
			temp |= list[i];
		}
		mDestructType = temp;
	}
}

/*******************************************************************************************************************

	INPUT & OUTPUT

*******************************************************************************************************************/

void ViAudioObject::setType(ViAudio::Type input, ViAudio::Type output)
{
	setInputType(input);
	setOutputType(output);
}

void ViAudioObject::setInputType(ViAudio::Type type)
{
	mInputType = type;
}

void ViAudioObject::setOutputType(ViAudio::Type type)
{
	mOutputType = type;
}

/*******************************************************************************************************************

	SLOTS

*******************************************************************************************************************/

void ViAudioObject::checkFinished()
{
	if(!isUsed(QIODevice::WriteOnly))
	{
		emit finished();
	}
}

/*******************************************************************************************************************

	BUFFERS

*******************************************************************************************************************/

ViBuffer* ViAudioObject::targetBuffer()
{
	QMutexLocker locker(&mMutex);
	if(mTargetBuffer == NULL)
	{
		mTargetBuffer = new ViBuffer();
		QObject::connect(mTargetBuffer, SIGNAL(unused()), this, SLOT(checkFinished()));
	}
	return mTargetBuffer;
}

ViBuffer* ViAudioObject::corruptedBuffer()
{
	QMutexLocker locker(&mMutex);
	if(mCorruptedBuffer == NULL)
	{
		mCorruptedBuffer = new ViBuffer();
		QObject::connect(mCorruptedBuffer, SIGNAL(unused()), this, SLOT(checkFinished()));
	}
	return mCorruptedBuffer;
}

ViBuffer* ViAudioObject::correctedBuffer()
{
	QMutexLocker locker(&mMutex);
	if(mCorrectedBuffer == NULL)
	{
		mCorrectedBuffer = new ViBuffer();
		QObject::connect(mCorrectedBuffer, SIGNAL(unused()), this, SLOT(checkFinished()));
	}
	return mCorrectedBuffer;
}

ViBuffer* ViAudioObject::tempBuffer()
{
	QMutexLocker locker(&mMutex);
	if(mTempBuffer == NULL)
	{
		mTempBuffer = new ViBuffer();
		QObject::connect(mTempBuffer, SIGNAL(unused()), this, SLOT(checkFinished()));
	}
	return mTempBuffer;
}

ViBuffer* ViAudioObject::inputBuffer()
{
	if(mInputType == ViAudio::TargetType)
	{
		return targetBuffer();
	}
	else if(mInputType == ViAudio::CorruptedType)
	{
		return corruptedBuffer();
	}
	else if(mInputType == ViAudio::CorrectedType)
	{
		return correctedBuffer();
	}
	else if(mInputType == ViAudio::TemporaryType)
	{
		return tempBuffer();
	}
	return NULL;
}

ViBuffer* ViAudioObject::outputBuffer()
{
	if(mOutputType == ViAudio::TargetType)
	{
		return targetBuffer();
	}
	else if(mOutputType == ViAudio::CorruptedType)
	{
		return corruptedBuffer();
	}
	else if(mOutputType == ViAudio::CorrectedType)
	{
		return correctedBuffer();
	}
	else if(mOutputType == ViAudio::TemporaryType)
	{
		return tempBuffer();
	}
	return NULL;
}

void ViAudioObject::setTargetBuffer(ViBuffer *buffer)
{
	QMutexLocker locker(&mMutex);
	mTargetBuffer = buffer;
}

void ViAudioObject::setCorruptedBuffer(ViBuffer *buffer)
{
	QMutexLocker locker(&mMutex);
	mCorruptedBuffer = buffer;
}

void ViAudioObject::setCorrectedBuffer(ViBuffer *buffer)
{
	QMutexLocker locker(&mMutex);
	mCorrectedBuffer = buffer;
}

void ViAudioObject::clearBuffers(ViAudio::Type type)
{
	if(mDestructType & ViAudio::TargetType)
	{
		clearTargetBuffer();
	}
	if(mDestructType & ViAudio::CorruptedType)
	{
		clearCorruptedBuffer();
	}
	if(mDestructType & ViAudio::CorrectedType)
	{
		clearCorrectedBuffer();
	}
	if(mDestructType & ViAudio::TemporaryType)
	{
		clearTempBuffer();
	}
}

void ViAudioObject::clearTargetBuffer()
{
	QMutexLocker locker(&mMutex);
	if(mTargetBuffer != NULL)
	{
		QObject::disconnect(mTargetBuffer, SIGNAL(unused()), this, SLOT(checkFinished()));
		delete mTargetBuffer;
		mTargetBuffer = NULL;
	}
}

void ViAudioObject::clearCorruptedBuffer()
{
	QMutexLocker locker(&mMutex);
	if(mCorruptedBuffer != NULL)
	{
		QObject::disconnect(mCorruptedBuffer, SIGNAL(unused()), this, SLOT(checkFinished()));
		delete mCorruptedBuffer;
		mCorruptedBuffer = NULL;
	}
}

void ViAudioObject::clearCorrectedBuffer()
{
	QMutexLocker locker(&mMutex);
	if(mCorrectedBuffer != NULL)
	{
		QObject::disconnect(mCorrectedBuffer, SIGNAL(unused()), this, SLOT(checkFinished()));
		delete mCorrectedBuffer;
		mCorrectedBuffer = NULL;
	}
}

void ViAudioObject::clearTempBuffer()
{
	QMutexLocker locker(&mMutex);
	if(mTempBuffer != NULL)
	{
		QObject::disconnect(mTempBuffer, SIGNAL(unused()), this, SLOT(checkFinished()));
		delete mTempBuffer;
		mTempBuffer = NULL;
	}
}

/*******************************************************************************************************************

	FILES

*******************************************************************************************************************/

QString ViAudioObject::filePath(ViAudio::Type type)
{
	if(type == ViAudio::TargetType)
	{
		return targetFilePath();
	}
	else if(type == ViAudio::CorruptedType)
	{
		return corruptedFilePath();
	}
	else if(type == ViAudio::CorrectedType)
	{
		return correctedFilePath();
	}
	return "";
}

QString ViAudioObject::targetFilePath()
{
	return mTargetFile;
}

QString ViAudioObject::corruptedFilePath()
{
	return mCorruptedFile;
}

QString ViAudioObject::correctedFilePath()
{
	return mCorrectedFile;
}

void ViAudioObject::setFilePath(ViAudio::Type type, QString path)
{
	if(type == ViAudio::TargetType)
	{
		setTargetFilePath(path);
	}
	else if(type == ViAudio::CorruptedType)
	{
		setCorruptedFilePath(path);
	}
	else if(type == ViAudio::CorrectedType)
	{
		setCorrectedFilePath(path);
	}
}

void ViAudioObject::setTargetFilePath(QString path)
{
	mTargetFile = path;
}

void ViAudioObject::setCorruptedFilePath(QString path)
{
	mCorruptedFile = path;
}

void ViAudioObject::setCorrectedFilePath(QString path)
{
	mCorrectedFile = path;
}

/*******************************************************************************************************************

	SONG INFO

*******************************************************************************************************************/

ViSongInfo& ViAudioObject::songInfo()
{
	return mSongInfo;
}

void ViAudioObject::setSongInfo(ViSongInfo info)
{
	mSongInfo = info;
}












void ViAudioObject::setSong(bool song)
{
	QMutexLocker locker(&mMutex);
	mIsSong = song;
}

bool ViAudioObject::isSong()
{
	QMutexLocker locker(&mMutex);
	return mIsSong;
}

bool ViAudioObject::isFinished()
{
	QMutexLocker locker(&mMutex);
	return mIsFinished;
}

bool ViAudioObject::isUsed(QIODevice::OpenMode mode)
{
	QMutexLocker locker(&mMutex);
	return	(mTargetBuffer != NULL && mTargetBuffer->streamCount(mode) > 0) ||
			(mCorruptedBuffer != NULL && mCorruptedBuffer->streamCount(mode) > 0) ||
			(mCorrectedBuffer != NULL && mCorrectedBuffer->streamCount(mode) > 0) ||
			(mTempBuffer != NULL && mTempBuffer->streamCount(mode) > 0);
}

void ViAudioObject::addCorrelation(const ViElement &correlation)
{
	mCorrelations.append(correlation);
}

ViElementList& ViAudioObject::correlations()
{
	return mCorrelations;
}
