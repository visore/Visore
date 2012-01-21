#ifndef VIAUDIODEVICE_H
#define VIAUDIODEVICE_H

#include "vierror.h"
#include <QString>
#include <QList>

class ViAudioDevice : public ViError
{
	public:
		enum ViAudioDeviceType
		{
			None = 0,
			All = 1,
			Input = 2,
			Output = 3
		};

		ViAudioDevice(int id = -1, QString name = "", QString description = "", QString driver = "", ViAudioDevice::ViAudioDeviceType type = ViAudioDevice::None, bool isDefault = false)
		{
			mId = id;
			mName = name;
			mDescription = description;
			mDriver = driver;
			mType = type;
			mIsDefault = isDefault;
		}
		
		int id()
		{
			return mId;
		}

		QString name()
		{
			return mName;
		}

		QString description()
		{
			return mDescription;
		}

		QString driver()
		{
			return mDriver;
		}

		ViAudioDevice::ViAudioDeviceType type()
		{
			return mType;
		}

		bool isDefault()
		{
			return mIsDefault;
		}

		void setId(int id)
		{
			mId = id;
		}

		void setName(QString name)
		{
			mName = name;
		}

		void setDescription(QString description)
		{
			mDescription = description;
		}

		void setDriver(QString driver)
		{
			mDriver = driver;
		}

		void setType(ViAudioDevice::ViAudioDeviceType type)
		{
			mType = type;
		}

		void setIsDefault(bool isDefault)
		{
			mIsDefault = isDefault;
		}

		static QList<ViAudioDevice> devices(ViAudioDevice::ViAudioDeviceType type = ViAudioDevice::All)
		{
			ViAudioDevice device;
			device.setErrorParameters("ViAudioDevice - Invalid Usage", "ViAudioDevice cannot be used, please used one of the derived classes", ViErrorInfo::NonFatal);
			QList<ViAudioDevice> devices;
			return devices;
		}

		static ViAudioDevice defaultDevice(ViAudioDevice::ViAudioDeviceType type = ViAudioDevice::Output)
		{
			ViAudioDevice device;
			device.setErrorParameters("ViAudioDevice - Invalid Usage", "ViAudioDevice cannot be used, please used one of the derived classes", ViErrorInfo::NonFatal);
			return device;
		}

	protected:
		int mId;
		QString mName;
		QString mDescription;
		QString mDriver;
		ViAudioDevice::ViAudioDeviceType mType;
		bool mIsDefault;
};

#endif
