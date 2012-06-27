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
}

void ViAudioTransmission::setFormat(ViAudioFormat format)
{
}

void ViAudioTransmission::setState(QAudio::State state)
{
	mState = state;
	emit stateChanged(mState);
}

QAudio::State ViAudioTransmission::state()
{
	return mState;
}
