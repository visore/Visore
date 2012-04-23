#include "viaudiooutput.h"

ViAudioOutput::ViAudioOutput(ViAudioFormat format, ViAudioBuffer *buffer)
	: ViAudioTransmission(format, buffer)
{
	mStream = mBuffer->createReadStream();
}
