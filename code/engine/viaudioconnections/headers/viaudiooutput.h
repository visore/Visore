#ifndef VIAUDIOOUTPUT_H
#define VIAUDIOOUTPUT_H

#include "viaudiotransmission.h"

class ViAudioOutput : public ViAudioTransmission
{
    Q_OBJECT

	public:

		ViAudioOutput();

		virtual void setBuffer(ViBuffer *buffer);

		virtual void start() = 0;
		virtual void stop() = 0;
		virtual void pause() = 0;

};

#endif
