#ifndef VIQTCONNECTION_H
#define VIQTCONNECTION_H

#include "viaudioconnection.h"
#include "viformatmanager.h"
#include "viqtfileinput.h"
#include "viqtstreaminput.h"
#include "viqtfileoutput.h"
#include "viqtstreamoutput.h"
#include "viqtdevice.h"

class ViQtConnection : public ViAudioConnection
{
	public:
		ViQtConnection();
		~ViQtConnection();
		ViFileInput* fileInput(ViAudioBuffer *buffer = NULL, ViAudioMetaData *metaData = NULL, QString filePath = "");
		ViStreamInput* streamInput(ViAudioBuffer *buffer = NULL, ViAudioMetaData *metaData = NULL, ViAudioDevice *device = NULL);
		ViFileOutput* fileOutput(ViAudioBuffer *buffer = NULL, ViAudioMetaData *metaData = NULL, QString filePath = "");
		ViStreamOutput* streamOutput(ViAudioBuffer *buffer = NULL, ViAudioMetaData *metaData = NULL, ViAudioDevice *device = NULL);
		QList<ViAudioDevice> devices(ViAudioDevice::ViAudioDeviceType type = ViAudioDevice::All);
		ViAudioDevice defaultDevice(ViAudioDevice::ViAudioDeviceType type = ViAudioDevice::Output);

	private:
		void initialize();
		void close();
		void populateInputFormats();
		void populateOutputFormats();

};

#endif
