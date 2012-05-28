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
