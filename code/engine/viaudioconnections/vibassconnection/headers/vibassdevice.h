#ifndef ViBASSDEVICE_H
#define ViBASSDEVICE_H

#include "viaudiodevice.h"
#include "bass.h"

class ViBassDevice : public ViAudioDevice
{
	public:
		ViBassDevice(int id = -1, QString name = "", QString description = "", QString driver = "", ViAudioDevice::ViAudioDeviceType type = ViAudioDevice::None, bool isDefault = false);
		static QList<ViAudioDevice> devices(ViAudioDevice::ViAudioDeviceType type = ViAudioDevice::All);
		static ViAudioDevice defaultDevice(ViAudioDevice::ViAudioDeviceType type = ViAudioDevice::Output);
};

#endif
