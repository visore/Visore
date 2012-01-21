#include "vibassdevice.h"

ViBassDevice::ViBassDevice(int id, QString name, QString description, QString driver, ViAudioDevice::ViAudioDeviceType type, bool isDefault)
	: ViAudioDevice(id, name, description, driver, type, isDefault)
{
}

QList<ViAudioDevice> ViBassDevice::devices(ViAudioDevice::ViAudioDeviceType type)
{
	QList<ViAudioDevice> outputDevices;
	QList<ViAudioDevice> inputDevices;
	BASS_DEVICEINFO info;
	if(type == ViAudioDevice::Output || type == ViAudioDevice::All)
	{
		for(int i = 0; BASS_GetDeviceInfo(i, &info); ++i)
		{
			int error = BASS_ErrorGetCode();
			if(error == BASS_ERROR_DEVICE)
			{
				ViAudioDevice device;
				device.setErrorParameters("ViBassDevice - Invalid Device", "An invalid output device was detected", ViErrorInfo::NonFatal);
			}
			else if(info.flags&BASS_DEVICE_ENABLED)
			{
				ViAudioDevice device;
				device.setId(i);
				device.setName(QString(info.name));
				device.setDriver(QString(info.driver));
				device.setType(ViAudioDevice::Output);
				if(info.flags&BASS_DEVICE_DEFAULT)
				{
					device.setIsDefault(true);
				}
				outputDevices.append(device);
			}
			delete info.name;
			delete info.driver;
		}
	}
	if(type == ViAudioDevice::Input || type == ViAudioDevice::All)
	{
		for(int i = 0; BASS_RecordGetDeviceInfo(i, &info); ++i)
		{
			int error = BASS_ErrorGetCode();
			if(error == BASS_ERROR_DX)
			{
				ViAudioDevice device;
				device.setErrorParameters("ViBassDevice - Insufficient DirectX", "An insufficient version of DirectX was detected. DirectX 5 (or above) is recuired on Windows systems", ViErrorInfo::NonFatal);
			}
			else if(error == BASS_ERROR_DEVICE)
			{
				ViAudioDevice device;
				device.setErrorParameters("ViBassDevice - Invalid Device", "An invalid input device was detected", ViErrorInfo::NonFatal);
			}
			else if(info.flags&BASS_DEVICE_ENABLED)
			{
				ViAudioDevice device;
				device.setId(i);
				device.setName(QString(info.name));
				device.setDriver(QString(info.driver));
				device.setType(ViAudioDevice::Input);
				if(info.flags&BASS_DEVICE_DEFAULT)
				{
					device.setIsDefault(true);
				}
				inputDevices.append(device);
			}
			delete info.name;
			delete info.driver;
		}
	}
	if(type == ViAudioDevice::Output)
	{
		return outputDevices;
	}
	else if(type == ViAudioDevice::Input)
	{
		return inputDevices;
	}
	else if(type == ViAudioDevice::None)
	{
		ViAudioDevice device;
		device.setErrorParameters("ViBassDevice - No Devices", "No suitable device type was selected. Please use Output, Input or All", ViErrorInfo::NonFatal);
	}
	outputDevices.append(inputDevices);
	return outputDevices;
}

ViAudioDevice ViBassDevice::defaultDevice(ViAudioDevice::ViAudioDeviceType type)
{
	BASS_DEVICEINFO info;
	ViAudioDevice device;
	if(type == ViAudioDevice::Output)
	{
		for(int i = 0; BASS_GetDeviceInfo(i, &info); ++i)
		{
			int error = BASS_ErrorGetCode();
			if(error == BASS_ERROR_DEVICE)
			{
				device.setErrorParameters("ViBassDevice - Invalid Device", "An invalid output device was detected", ViErrorInfo::NonFatal);
			}
			else if(info.flags&BASS_DEVICE_ENABLED && info.flags&BASS_DEVICE_DEFAULT)
			{
				device.setId(i);
				device.setName(QString(info.name));
				device.setDriver(QString(info.driver));
				device.setType(ViAudioDevice::Output);
				if(info.flags&BASS_DEVICE_DEFAULT)
				{
					device.setIsDefault(true);
				}
			}
			delete info.name;
			delete info.driver;
		}
	}
	else if(type == ViAudioDevice::Input)
	{
		for(int i = 0; BASS_RecordGetDeviceInfo(i, &info); ++i)
		{
			int error = BASS_ErrorGetCode();
			if(error == BASS_ERROR_DX)
			{
				device.setErrorParameters("ViBassDevice - Insufficient DirectX", "An insufficient version of DirectX was detected. DirectX 5 (or above) is recuired on Windows systems", ViErrorInfo::NonFatal);
			}
			else if(error == BASS_ERROR_DEVICE)
			{
				device.setErrorParameters("ViBassDevice - Invalid Device", "An invalid input device was detected", ViErrorInfo::NonFatal);
			}
			else if(info.flags&BASS_DEVICE_ENABLED && info.flags&BASS_DEVICE_DEFAULT)
			{
				device.setId(i);
				device.setName(QString(info.name));
				device.setDriver(QString(info.driver));
				device.setType(ViAudioDevice::Input);
				if(info.flags&BASS_DEVICE_DEFAULT)
				{
					device.setIsDefault(true);
				}
			}
			delete info.name;
			delete info.driver;
		}
	}
	if(device.type() == ViAudioDevice::None)
	{
		device.setErrorParameters("ViBassDevice - No Device", "No suitable device type was selected. Please use Output or Input", ViErrorInfo::NonFatal);
	}
	return device;
}
