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

		ViAudioDevice(int id = -1, QString name = "", QString description = "", QString driver = "", ViAudioDevice::ViAudioDeviceType type = ViAudioDevice::None, bool isDefault = false);
		int id();
		QString name();
		QString description();
		QString driver();
		ViAudioDevice::ViAudioDeviceType type();
		bool isDefault();
		void setId(int id);
		void setName(QString name);
		void setDescription(QString description);
		void setDriver(QString driver);
		void setType(ViAudioDevice::ViAudioDeviceType type);
		void setIsDefault(bool isDefault);
		static QList<ViAudioDevice> devices(ViAudioDevice::ViAudioDeviceType type = ViAudioDevice::All);
		static ViAudioDevice defaultDevice(ViAudioDevice::ViAudioDeviceType type = ViAudioDevice::Output);

	protected:
		int mId;
		QString mName;
		QString mDescription;
		QString mDriver;
		ViAudioDevice::ViAudioDeviceType mType;
		bool mIsDefault;
};

#endif
