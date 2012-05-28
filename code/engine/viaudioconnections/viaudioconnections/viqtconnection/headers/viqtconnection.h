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

		ViFileInput* fileInput();
		ViStreamInput* streamInput();
		ViFileOutput* fileOutput();
		ViStreamOutput* streamOutput();
};

#endif
