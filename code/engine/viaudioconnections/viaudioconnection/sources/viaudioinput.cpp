#include "viaudioinput.h"

ViAudioInput::ViAudioInput(ViAudioBuffer *buffer, ViAudioMetaData *metaData)
	: ViAudioTransmission(buffer, metaData)
{
	mStream = mBuffer->createWriteStream();
}
