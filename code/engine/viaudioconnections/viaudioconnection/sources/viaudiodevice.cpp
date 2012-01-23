#include "viaudiodevice.h"

ViAudioDevice::ViAudioDevice(int id, QString name, QString description, QString driver, ViAudioDevice::ViAudioDeviceType type, bool isDefault)
{
	mId = id;
	mName = name;
	mDescription = description;
	mDriver = driver;
	mType = type;
	mIsDefault = isDefault;
}
		
int ViAudioDevice::id()
{
	return mId;
}

QString ViAudioDevice::name()
{
	return mName;
}

QString ViAudioDevice::description()
{
	return mDescription;
}

QString ViAudioDevice::driver()
{
	return mDriver;
}

ViAudioDevice::ViAudioDeviceType ViAudioDevice::type()
{
	return mType;
}

bool ViAudioDevice::isDefault()
{
	return mIsDefault;
}

void ViAudioDevice::setId(int id)
{
	mId = id;
}

void ViAudioDevice::setName(QString name)
{
	mName = name;
}

void ViAudioDevice::setDescription(QString description)
{
	mDescription = description;
}

void ViAudioDevice::setDriver(QString driver)
{
	mDriver = driver;
}

void ViAudioDevice::setType(ViAudioDevice::ViAudioDeviceType type)
{
	mType = type;
}

void ViAudioDevice::setIsDefault(bool isDefault)
{
	mIsDefault = isDefault;
}

QList<ViAudioDevice> ViAudioDevice::devices(ViAudioDevice::ViAudioDeviceType type)
{
	ViAudioDevice device;
	device.setErrorParameters("ViAudioDevice - Invalid Usage", "ViAudioDevice cannot be used, please used one of the derived classes", ViErrorInfo::NonFatal);
	QList<ViAudioDevice> devices;
	return devices;
}

ViAudioDevice ViAudioDevice::defaultDevice(ViAudioDevice::ViAudioDeviceType type)
{
	ViAudioDevice device;
	device.setErrorParameters("ViAudioDevice - Invalid Usage", "ViAudioDevice cannot be used, please used one of the derived classes", ViErrorInfo::NonFatal);
	return device;
}
