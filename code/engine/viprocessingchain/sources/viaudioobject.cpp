#include "viaudioobject.h"

ViAudioObject::ViAudioObject(bool autoDestruct)
	: QObject()
{
	mAutoDestruct = autoDestruct;
	mIsFinished = false;
	mIsSong = false;
	mOriginalBuffer = NULL;
	mCorrectedBuffer = NULL;
}

ViAudioObject::ViAudioObject(ViAudioBuffer *original, ViAudioBuffer *corrected, bool autoDestruct)
	: QObject()
{
	mAutoDestruct = autoDestruct;
	mIsSong = false;
	setBuffers(original, corrected);
}

ViAudioObject::~ViAudioObject()
{
	if(mAutoDestruct)
	{
		clearBuffers();
	}
}

void ViAudioObject::setSong(bool song)
{
	QMutexLocker locker(&mMutex);
	mIsSong = song;
}
		
void ViAudioObject::setBuffers(ViAudioBuffer *original, ViAudioBuffer *corrected)
{
	setOriginalBuffer(original);
	setCorrectedBuffer(corrected);
}

void ViAudioObject::setOriginalBuffer(ViAudioBuffer *buffer)
{
	QMutexLocker locker(&mMutex);
	mOriginalBuffer = buffer;
}

void ViAudioObject::setCorrectedBuffer(ViAudioBuffer *buffer)
{
	QMutexLocker locker(&mMutex);
	mCorrectedBuffer = buffer;
}

bool ViAudioObject::isSong()
{
	QMutexLocker locker(&mMutex);
	return mIsSong;
}

void ViAudioObject::clearBuffers()
{
	clearOriginalBuffer();
	clearCorrectedBuffer();
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

ViAudioBuffer* ViAudioObject::originalBuffer()
{
	QMutexLocker locker(&mMutex);
	return mOriginalBuffer;
}

ViAudioBuffer* ViAudioObject::correctedBuffer()
{
	QMutexLocker locker(&mMutex);
	return mCorrectedBuffer;
}

void ViAudioObject::setFinished(bool isFinished)
{
	QMutexLocker locker(&mMutex);
	mIsFinished = isFinished;
	if(mIsFinished)
	{
		emit finished();
	}
}

bool ViAudioObject::isFinished()
{
	QMutexLocker locker(&mMutex);
	return mIsFinished;
}
