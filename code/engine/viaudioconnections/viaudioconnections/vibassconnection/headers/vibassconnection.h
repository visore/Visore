#ifndef VIBASSCONNECTION_H
#define VIBASSCONNECTION_H

#include "viaudioconnection.h"
#include "viaudioformatmanager.h"
#include "vibassfileinput.h"
#include "vibassstreaminput.h"
//#include "vibassfileoutput.h"
#include "vibassstreamoutput.h"
#include "vibassdevice.h"

class ViBassConnection : public ViAudioConnection
{
	public:
		ViBassConnection();
		~ViBassConnection();
		ViFileInput* fileInput(ViAudioBuffer *buffer = NULL, ViAudioMetaData *metaData = NULL, QString filePath = "");
		ViStreamInput* streamInput(ViAudioBuffer *buffer = NULL, ViAudioMetaData *metaData = NULL, ViAudioDevice *device = NULL);
		//ViFileOutput* fileOutput(ViAudioBuffer *buffer = NULL, ViAudioMetaData *metaData = NULL, QString filePath);
		ViStreamOutput* streamOutput(ViAudioBuffer *buffer = NULL, ViAudioMetaData *metaData = NULL, ViAudioDevice *device = NULL);
		QList<ViAudioDevice> devices(ViAudioDevice::ViAudioDeviceType type = ViAudioDevice::All);
		ViAudioDevice defaultDevice(ViAudioDevice::ViAudioDeviceType type = ViAudioDevice::Output);

	private:
		void initialize();
		void close();
		void populateInputFormats();
		void populateOutputFormats();

	private:
		QList<HPLUGIN> mPlugins;
		ViAudioFormatManager *mFormatManager;
};

#endif
