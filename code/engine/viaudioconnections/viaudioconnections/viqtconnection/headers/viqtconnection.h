#ifndef VIQTCONNECTION_H
#define VIQTCONNECTION_H

#include <QAudioDeviceInfo>
#include "viaudioconnection.h"
#include "viqtfileinput.h"
#include "viqtstreaminput.h"
#include "viqtfileoutput.h"
#include "viqtstreamoutput.h"

class ViQtConnection : public ViAudioConnection
{
	public:
		ViQtConnection();
		~ViQtConnection();

		ViFileInput* fileInput(ViAudioFormat format, ViAudioBuffer *buffer, QString filePath = "");
		ViStreamInput* streamInput(ViAudioFormat format, ViAudioBuffer *buffer, QAudioDeviceInfo device);
		ViFileOutput* fileOutput(ViAudioFormat format, ViAudioBuffer *buffer, QString filePath = "");
		ViStreamOutput* streamOutput(ViAudioFormat format, ViAudioBuffer *buffer, QAudioDeviceInfo device);

	private:
		void initialize();
		void close();

};

#endif
