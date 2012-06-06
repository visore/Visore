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

		enum Direction
		{
			Input = 0,
			Output = 1
		};

		enum Type
		{
			Stream = 0,
			File = 1
		};

		ViAudioConnection();
		~ViAudioConnection();

		virtual ViFileInput* fileInput(ViAudioFormat format, ViAudioBuffer *buffer, QString filePath = "") = 0;
		virtual ViStreamInput* streamInput(ViAudioFormat format, ViAudioBuffer *buffer, QAudioDeviceInfo device) = 0;
		virtual ViFileOutput* fileOutput(ViAudioFormat format, ViAudioBuffer *buffer, QString filePath = "") = 0;
		virtual ViStreamOutput* streamOutput(ViAudioFormat format, ViAudioBuffer *buffer, QAudioDeviceInfo device) = 0;

	protected:

		void close(ViAudioTransmission *transmission);

	protected:

		ViFileInput *mFileInput;
		ViStreamInput *mStreamInput;
		ViFileOutput *mFileOutput;
		ViStreamOutput *mStreamOutput;

};

#endif
