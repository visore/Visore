#include "viaudiooutput.h"

ViAudioOutput::ViAudioOutput()
	: ViAudioTransmission()
{
}

void ViAudioOutput::setBuffer(ViAudioBuffer *buffer)
{
	ViAudioTransmission::setBuffer(buffer);
	mStream = mBuffer->createReadStream();
}
