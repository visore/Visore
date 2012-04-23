#ifndef VIAUDIOCONNECTION_H
#define VIAUDIOCONNECTION_H

#include "vierror.h"
#include "vifileinput.h"
#include "vistreaminput.h"
#include "vifileoutput.h"
#include "vistreamoutput.h"

class ViAudioConnection : public ViError
{
	public:
		ViAudioConnection();
		~ViAudioConnection();
		QList<ViAudioFormat*> supportedInputFormats();
		QList<ViAudioFormat*> supportedOutputFormats();

		virtual ViFileInput* fileInput(ViAudioFormat format, ViAudioBuffer *buffer = NULL, QString filePath = "") = 0;
		virtual ViStreamInput* streamInput(ViAudioFormat format, ViAudioBuffer *buffer = NULL, ViAudioDevice *device = NULL) = 0;
		virtual ViFileOutput* fileOutput(ViAudioFormat format, ViAudioBuffer *buffer = NULL, QString filePath = "") = 0;
		virtual ViStreamOutput* streamOutput(ViAudioFormat format, ViAudioBuffer *buffer = NULL, ViAudioDevice *device = NULL) = 0;
		virtual QList<ViAudioDevice> devices(ViAudioDevice::ViAudioDeviceType type = ViAudioDevice::All) = 0;
		virtual ViAudioDevice defaultDevice(ViAudioDevice::ViAudioDeviceType type = ViAudioDevice::Output) = 0;

	protected:
		ViFileInput *mFileInput;
		ViStreamInput *mStreamInput;
		ViFileOutput *mFileOutput;
		ViStreamOutput *mStreamOutput;
		QList<ViAudioFormat*> mInputFormats;
		QList<ViAudioFormat*> mOutputFormats;
};

#endif
