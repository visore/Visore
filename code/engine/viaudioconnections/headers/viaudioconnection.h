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

		ViFileInput* fileInput();
		ViStreamInput* streamInput();
		ViFileOutput* fileOutput();
		ViStreamOutput* streamOutput();

	protected:

		ViFileInput *mFileInput;
		ViStreamInput *mStreamInput;
		ViFileOutput *mFileOutput;
		ViStreamOutput *mStreamOutput;

};

#endif
