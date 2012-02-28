#include "vistreamoutput.h"

ViStreamOutput::ViStreamOutput(ViAudioBuffer *buffer, ViAudioMetaData* metaData, ViAudioDevice *device)
	: ViAudioOutput(buffer, metaData)
{
	mDevice = device;
	mVolume = 0;
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

void ViStreamOutput::bufferChanged(int size)
{
}
