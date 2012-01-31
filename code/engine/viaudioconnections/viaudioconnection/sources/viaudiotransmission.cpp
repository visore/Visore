#include "viaudiotransmission.h"

ViAudioTransmission::ViAudioTransmission(ViAudioBuffer *buffer, ViAudioMetaData *metaData)
	: QObject()
{
	mBuffer = buffer;
	mStream = NULL;
	mMetaData = metaData;
	mStatus = ViAudioTransmission::None;
}

ViAudioTransmission::~ViAudioTransmission()
{
	if(mStream != NULL)
	{
		delete mStream;
		mStream = NULL;
	}
	if(mMetaData != NULL)
	{
		delete mMetaData;
		mMetaData = NULL;
	}
}

void ViAudioTransmission::setBuffer(ViAudioBuffer *buffer)
{
	mBuffer = buffer;
}

ViAudioMetaData* ViAudioTransmission::metaData()
{
	return mMetaData;
}

void ViAudioTransmission::setMetaData(ViAudioMetaData *metaData)
{
	mMetaData = metaData;
}

void ViAudioTransmission::setStatus(ViAudioTransmission::ViTransmissionStatus status)
{
	mStatus = status;
}

ViAudioTransmission::ViTransmissionStatus ViAudioTransmission::status()
{
	return mStatus;
}
