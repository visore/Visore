#ifndef ViQTDEVICE_H
#define ViQTDEVICE_H

#include "viaudiodevice.h"

class ViQtDevice : public ViAudioDevice
{
	public:
		ViQtDevice(int id = -1, QString name = "", QString description = "", QString driver = "", ViAudioDevice::ViAudioDeviceType type = ViAudioDevice::None, bool isDefault = false);
		static QList<ViAudioDevice> devices(ViAudioDevice::ViAudioDeviceType type = ViAudioDevice::All);
		static ViAudioDevice defaultDevice(ViAudioDevice::ViAudioDeviceType type = ViAudioDevice::Output);
};

#endif
