#include "viqtdevice.h"

ViQtDevice::ViQtDevice(int id, QString name, QString description, QString driver, ViAudioDevice::ViAudioDeviceType type, bool isDefault)
	: ViAudioDevice(id, name, description, driver, type, isDefault)
{
}

QList<ViAudioDevice> ViQtDevice::devices(ViAudioDevice::ViAudioDeviceType type)
{

}

ViAudioDevice ViQtDevice::defaultDevice(ViAudioDevice::ViAudioDeviceType type)
{

}
