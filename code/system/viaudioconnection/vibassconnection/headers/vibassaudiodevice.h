#ifndef VIBASSAUDIODEVICE_H
#define VIBASSAUDIODEVICE_H

#include "viaudiodevice.h"
#include "bass.h"

class ViBassAudioDevice : public ViAudioDevice
{
	public:
		ViBassAudioDevice(QString name = "", QString description = "", QString driver = "", ViAudioDevice::ViAudioDeviceType type = ViAudioDevice::None, bool isDefault = false);
		static QList<ViAudioDevice> drivers(ViAudioDevice::ViAudioDeviceType type = ViAudioDevice::All);
		static ViAudioDevice defaultDriver(ViAudioDevice::ViAudioDeviceType type = ViAudioDevice::Output);
};

#endif
