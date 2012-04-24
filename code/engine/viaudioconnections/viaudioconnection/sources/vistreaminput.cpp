#include "vistreaminput.h"

ViStreamInput::ViStreamInput()
	: ViAudioInput()
{
	mDevice = QAudioDeviceInfo::defaultInputDevice();
}

void ViStreamInput::setDevice(QAudioDeviceInfo device)
{
	mDevice = device;
}

void ViStreamInput::initialize()
{
	ViAudioInput::initialize();
}

void ViStreamInput::free()
{
	ViAudioInput::free();
}
