#ifndef VIAUDIOCONNECTION_H
#define VIAUDIOCONNECTION_H

#include "vifileinput.h"
#include "vistreaminput.h"
#include "vifileoutput.h"
#include "vistreamoutput.h"

class ViAudioConnection
{

	public:

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
