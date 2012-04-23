#include "viaudioinput.h"

ViAudioInput::ViAudioInput(ViAudioFormat format, ViAudioBuffer *buffer)
	: ViAudioTransmission(format, buffer)
{
	mStream = mBuffer->createWriteStream();
}
