#include "viaudioobject.h"

ViAudioObject::ViAudioObject(bool autoDestruct)
	: QObject()
{
	mAutoDestruct = autoDestruct;
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
	if(mOriginalBuffer != NULL)
	{
		delete mOriginalBuffer;
		mOriginalBuffer = NULL;
	}
	if(mCorrectedBuffer != NULL)
	{
		delete mCorrectedBuffer;
		mCorrectedBuffer = NULL;
	}
}
