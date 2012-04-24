#include "viaudiooutput.h"

ViAudioOutput::ViAudioOutput()
	: ViAudioTransmission()
{
}

void ViAudioOutput::initialize()
{
	ViAudioOutput::free();
	mStream = mBuffer->createReadStream();
}

void ViAudioOutput::free()
{
	if(mStream != NULL)
	{
		delete mStream;
		mStream = NULL;
	}
}

void ViAudioOutput::changeFormat(ViAudioFormat format)
{
	setFormat(format);
	free();
	initialize();
}
