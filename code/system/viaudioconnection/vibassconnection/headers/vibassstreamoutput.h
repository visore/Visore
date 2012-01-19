#ifndef VIBASSSTREAMOUTPUT_H
#define VIBASSSTREAMOUTPUT_H

#include "viaudiooutput.h"
#include "bass.h"

class ViBassStreamOutput : public ViAudioOutput
{
	Q_OBJECT

	public:
		ViBassStreamOutput();
		~ViBassStreamOutput();
		virtual void start();

};

#endif
