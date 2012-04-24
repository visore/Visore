#include "viaudiotransmission.h"

ViAudioTransmission::ViAudioTransmission()
{
	mBuffer = NULL;
	mStream = NULL;
	mState = QAudio::IdleState;
}

ViAudioTransmission::~ViAudioTransmission()
{
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

void ViAudioTransmission::setState(QAudio::State state)
{
	mState = state;
}

QAudio::State ViAudioTransmission::state()
{
	return mState;
}
