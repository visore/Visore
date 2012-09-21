#include "viaudioobject.h"

ViAudioObject::ViAudioObject(bool autoDestruct)
	: QObject()
{
	mAutoDestruct = autoDestruct;
	mIsFinished = false;
	mOriginalBuffer = NULL;
	mCorrectedBuffer = NULL;
}

ViAudioObject::ViAudioObject(ViAudioBuffer *original, ViAudioBuffer *corrected, bool autoDestruct)
	: QObject()
{
	mAutoDestruct = autoDestruct;
	setBuffers(original, corrected);
}

ViAudioObject::~ViAudioObject()
{
	if(mAutoDestruct)
	{
		clearBuffers();
	}
}
		
void ViAudioObject::setBuffers(ViAudioBuffer *original, ViAudioBuffer *corrected)
{
	setOriginalBuffer(original);
	setCorrectedBuffer(corrected);
}

void ViAudioObject::setOriginalBuffer(ViAudioBuffer *buffer)
{
	mOriginalBuffer = buffer;
}

void ViAudioObject::setCorrectedBuffer(ViAudioBuffer *buffer)
{
	mCorrectedBuffer = buffer;
}

void ViAudioObject::clearBuffers()
{
	clearOriginalBuffer();
	clearCorrectedBuffer();
}

void ViAudioObject::clearOriginalBuffer()
{
	if(mOriginalBuffer != NULL)
	{
		delete mOriginalBuffer;
		mOriginalBuffer = NULL;
	}
}

void ViAudioObject::clearCorrectedBuffer()
{
	if(mCorrectedBuffer != NULL)
	{
		delete mCorrectedBuffer;
		mCorrectedBuffer = NULL;
	}
}

ViAudioBuffer* ViAudioObject::originalBuffer()
{
	return mOriginalBuffer;
}

ViAudioBuffer* ViAudioObject::correctedBuffer()
{
	return mCorrectedBuffer;
}

void ViAudioObject::setFinished(bool isFinished)
{
	mIsFinished = isFinished;
	if(mIsFinished)
	{
		emit finished();
	}
}

bool ViAudioObject::isFinished()
{
	return mIsFinished;
}
