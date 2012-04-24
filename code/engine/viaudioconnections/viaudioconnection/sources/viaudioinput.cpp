#include "viaudioinput.h"

ViAudioInput::ViAudioInput()
	: ViAudioTransmission()
{
}

void ViAudioInput::initialize()
{
	ViAudioInput::free();
	mStream = mBuffer->createWriteStream();
}

void ViAudioInput::free()
{
	if(mStream != NULL)
	{
		delete mStream;
		mStream = NULL;
	}
}
