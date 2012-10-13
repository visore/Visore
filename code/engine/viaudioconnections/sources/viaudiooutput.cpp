#include "viaudiooutput.h"

ViAudioOutput::ViAudioOutput()
	: ViAudioTransmission()
{
}

void ViAudioOutput::setBuffer(ViBuffer *buffer)
{
	ViAudioTransmission::setBuffer(buffer);
	mStream = mBuffer->createReadStream();
}
