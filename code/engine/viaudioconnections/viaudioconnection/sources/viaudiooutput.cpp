#include "viaudiooutput.h"

ViAudioOutput::ViAudioOutput(ViAudioBuffer *buffer, ViAudioMetaData *metaData)
	: ViAudioTransmission(buffer, metaData)
{
	mStream = mBuffer->createReadStream();
}
