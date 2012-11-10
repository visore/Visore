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

ViAudioObject::ViAudioObject(ViBuffer *original, ViBuffer *corrected, bool autoDestruct)
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
