#include "viaudiotransmission.h"

ViAudioTransmission::ViAudioTransmission(ViAudioFormat format, ViAudioBuffer *buffer)
	: QObject()
{
	mFormat = format;
	mBuffer = buffer;
	mStream = NULL;
	mStatus = ViAudioTransmission::None;
}

ViAudioTransmission::~ViAudioTransmission()
{
	if(mStream != NULL)
	{
		delete mStream;
		mStream = NULL;
	}
}

void ViAudioTransmission::setBuffer(ViAudioBuffer *buffer)
{
	mBuffer = buffer;
}

ViAudioBuffer* ViAudioTransmission::buffer()
{
	return mBuffer;
}

ViAudioFormat ViAudioTransmission::format()
{
	return mFormat;
}

void ViAudioTransmission::setFormat(ViAudioFormat format)
{
	mFormat = format;
}

void ViAudioTransmission::setStatus(ViAudioTransmission::ViTransmissionStatus status)
{
	mStatus = status;
}

ViAudioTransmission::ViTransmissionStatus ViAudioTransmission::status()
{
	return mStatus;
}
