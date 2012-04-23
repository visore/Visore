#include "vistreaminput.h"

ViStreamInput::ViStreamInput(ViAudioFormat format, ViAudioBuffer *buffer, ViAudioDevice *device)
	: ViAudioInput(format, buffer)
{
	mDevice = device;
}

ViStreamInput::~ViStreamInput()
{
	if(mDevice != NULL)
	{
		delete mDevice;
		mDevice = NULL;
	}
}

void ViStreamInput::setDevice(ViAudioDevice *device)
{
	mDevice = device;
}
