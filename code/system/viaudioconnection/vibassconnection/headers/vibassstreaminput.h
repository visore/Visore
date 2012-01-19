#ifndef VIBASSSTREAMINPUT_H
#define VIBASSSTREAMINPUT_H

#include "viaudioinput.h"
#include "bass.h"

class ViBassStreamInput : public ViAudioInput
{
	Q_OBJECT

	public:
		ViBassStreamInput();
		~ViBassStreamInput();
		virtual void start();

};

#endif
