#include "viaudioobject.h"

/*******************************************************************************************************************

	CONSTRUCTORS & DESTRUCTORS

*******************************************************************************************************************/

ViAudioObject::ViAudioObject(bool autoDestruct)
	: QObject()
{
	mAutoDestruct = autoDestruct;
	mIsFinished = false;
	mIsSong = false;

	mTargetBuffer = NULL;
	mOriginalBuffer = NULL;
	mCorrectedBuffer = NULL;

	mTargetFile = "";
	mOriginalFile = "";
	mCorrectedFile = "";
}

ViAudioObject::ViAudioObject(ViBuffer *original, ViBuffer *corrected, bool autoDestruct)
	: QObject()
{
	mAutoDestruct = autoDestruct;
	mIsSong = false;
	setBuffers(original, corrected);
	mTargetBuffer = NULL;
}

ViAudioObject::~ViAudioObject()
{
	if(mAutoDestruct)
	{
		clearBuffers();
	}
}

ViAudioObjectPointer ViAudioObject::create(ViAudioObject *object)
{
	return ViAudioObjectPointer(object);
}

ViAudioObjectPointer ViAudioObject::create(bool autoDestruct)
{
	return ViAudioObjectPointer(new ViAudioObject(autoDestruct));
}

ViAudioObjectPointer ViAudioObject::create(ViBuffer *original, ViBuffer *corrected, bool autoDestruct)
{
	return ViAudioObjectPointer(new ViAudioObject(original, corrected, autoDestruct));
}

ViAudioObjectPointer ViAudioObject::createNull()
{
	return ViAudioObjectPointer(NULL);
}

/*******************************************************************************************************************

	BUFFERS

*******************************************************************************************************************/

ViBuffer* ViAudioObject::targetBuffer()
{
	QMutexLocker locker(&mMutex);
	return mTargetBuffer;
}

ViBuffer* ViAudioObject::originalBuffer()
{
	QMutexLocker locker(&mMutex);
	return mOriginalBuffer;
}

ViBuffer* ViAudioObject::correctedBuffer()
{
	QMutexLocker locker(&mMutex);
	return mCorrectedBuffer;
}

void ViAudioObject::setTargetBuffer(ViBuffer *buffer)
{
	QMutexLocker locker(&mMutex);
	mTargetBuffer = buffer;
}

void ViAudioObject::setOriginalBuffer(ViBuffer *buffer)
{
	QMutexLocker locker(&mMutex);
	mOriginalBuffer = buffer;
}

void ViAudioObject::setCorrectedBuffer(ViBuffer *buffer)
{
	QMutexLocker locker(&mMutex);
	mCorrectedBuffer = buffer;
}

void ViAudioObject::clearBuffers()
{
	clearTargetBuffer();
	clearOriginalBuffer();
	clearCorrectedBuffer();
}

void ViAudioObject::clearTargetBuffer()
{
	QMutexLocker locker(&mMutex);
	if(mTargetBuffer != NULL)
	{
		delete mTargetBuffer;
		mTargetBuffer = NULL;
	}
}

void ViAudioObject::clearOriginalBuffer()
{
	QMutexLocker locker(&mMutex);
	if(mOriginalBuffer != NULL)
	{
		delete mOriginalBuffer;
		mOriginalBuffer = NULL;
	}
}

void ViAudioObject::clearCorrectedBuffer()
{
	QMutexLocker locker(&mMutex);
	if(mCorrectedBuffer != NULL)
	{
		delete mCorrectedBuffer;
		mCorrectedBuffer = NULL;
	}
}

/*******************************************************************************************************************

	FILES

*******************************************************************************************************************/

QString ViAudioObject::targetFile()
{
	return mTargetFile;
}

QString ViAudioObject::originalFile()
{
	return mOriginalFile;
}

QString ViAudioObject::correctedFile()
{
	return mCorrectedFile;
}

void ViAudioObject::setTargetFile(QString path)
{
	mTargetFile = path;
}

void ViAudioObject::setOriginalFile(QString path)
{
	mOriginalFile = path;
}

void ViAudioObject::setCorrectedFile(QString path)
{
	mCorrectedFile = path;
}

/*******************************************************************************************************************

	SONG INFO

*******************************************************************************************************************/

ViSongInfo ViAudioObject::songInfo()
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
		
void ViAudioObject::setBuffers(ViBuffer *original, ViBuffer *corrected)
{
	setOriginalBuffer(original);
	setCorrectedBuffer(corrected);
}

bool ViAudioObject::isSong()
{
	QMutexLocker locker(&mMutex);
	return mIsSong;
}


void ViAudioObject::setFinished(bool isFinished)
{
	QMutexLocker locker(&mMutex);
	mIsFinished = isFinished;
	if(mIsFinished)
	{
		locker.unlock();
		emit finished();
	}
}

bool ViAudioObject::isFinished()
{
	QMutexLocker locker(&mMutex);
	return mIsFinished;
}

void ViAudioObject::addCorrelation(const ViElement &correlation)
{
	mCorrelations.append(correlation);
}

ViElementList& ViAudioObject::correlations()
{
	return mCorrelations;
}
