#include "vistreamoutput.h"

ViStreamOutput::ViStreamOutput(ViAudioBuffer *buffer, ViAudioMetaData* metaData, ViAudioDevice *device)
	: ViAudioOutput(buffer, metaData)
{
	mDevice = device;
}

ViStreamOutput::~ViStreamOutput()
{
	if(mDevice != NULL)
	{
		delete mDevice;
		mDevice = NULL;
	}
}

void ViStreamOutput::setDevice(ViAudioDevice *device)
{
	mDevice = device;
}
