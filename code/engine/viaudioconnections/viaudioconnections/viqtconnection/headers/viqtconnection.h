#ifndef VIQTCONNECTION_H
#define VIQTCONNECTION_H

#include "viaudioconnection.h"
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
		ViFileInput* fileInput(ViAudioFormat format, ViAudioBuffer *buffer = NULL, QString filePath = "");
		ViStreamInput* streamInput(ViAudioFormat format, ViAudioBuffer *buffer = NULL, ViAudioDevice *device = NULL);
		ViFileOutput* fileOutput(ViAudioFormat format, ViAudioBuffer *buffer = NULL, QString filePath = "");
		ViStreamOutput* streamOutput(ViAudioFormat format, ViAudioBuffer *buffer = NULL, ViAudioDevice *device = NULL);
		QList<ViAudioDevice> devices(ViAudioDevice::ViAudioDeviceType type = ViAudioDevice::All);
		ViAudioDevice defaultDevice(ViAudioDevice::ViAudioDeviceType type = ViAudioDevice::Output);

	private:
		void initialize();
		void close();
		void populateInputFormats();
		void populateOutputFormats();

};

#endif
