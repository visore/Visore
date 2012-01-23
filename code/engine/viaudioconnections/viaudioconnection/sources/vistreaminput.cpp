#include "vistreaminput.h"

ViStreamInput::ViStreamInput(ViAudioBuffer *buffer, ViAudioMetaData *metaData, ViAudioDevice *device)
	: ViAudioInput(buffer, metaData)
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
