#include "viaudioinput.h"

ViAudioInput::ViAudioInput()
	: ViAudioTransmission()
{
}

void ViAudioInput::setBuffer(ViAudioBuffer *buffer)
{
	ViAudioTransmission::setBuffer(buffer);
	mStream = mBuffer->createWriteStream();
}
