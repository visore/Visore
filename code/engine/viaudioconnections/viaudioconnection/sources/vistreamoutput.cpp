#include "vistreamoutput.h"

ViStreamOutput::ViStreamOutput()
	: ViAudioOutput()
{
	mDevice = QAudioDeviceInfo::defaultOutputDevice();
	mVolume = 0;
}

void ViStreamOutput::setDevice(QAudioDeviceInfo device)
{
	mDevice = device;
}

void ViStreamOutput::bufferChanged(int size)
{
}

void ViStreamOutput::initialize()
{
	ViAudioOutput::initialize();
}

void ViStreamOutput::free()
{
	ViAudioOutput::free();
}
