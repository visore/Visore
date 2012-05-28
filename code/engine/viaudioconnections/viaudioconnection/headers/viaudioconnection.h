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

		virtual ViFileInput* fileInput() = 0;
		virtual ViStreamInput* streamInput() = 0;
		virtual ViFileOutput* fileOutput() = 0;
		virtual ViStreamOutput* streamOutput() = 0;

	protected:

		ViFileInput *mFileInput;
		ViStreamInput *mStreamInput;
		ViFileOutput *mFileOutput;
		ViStreamOutput *mStreamOutput;

};

#endif
