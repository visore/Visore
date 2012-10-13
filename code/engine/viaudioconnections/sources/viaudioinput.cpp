#include "viaudioinput.h"

ViAudioInput::ViAudioInput()
	: ViAudioTransmission()
{
}

void ViAudioInput::setBuffer(ViBuffer *buffer)
{
	ViAudioTransmission::setBuffer(buffer);
	mStream = mBuffer->createWriteStream();
}
