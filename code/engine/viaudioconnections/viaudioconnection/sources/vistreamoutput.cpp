#include "vistreamoutput.h"

ViStreamOutput::ViStreamOutput(ViAudioFormat format, ViAudioBuffer *buffer, ViAudioDevice *device)
	: ViAudioOutput(format, buffer)
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
