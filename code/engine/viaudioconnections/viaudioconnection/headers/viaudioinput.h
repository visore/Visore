#ifndef VIAUDIOINPUT_H
#define VIAUDIOINPUT_H

#include "viaudiotransmission.h"

class ViAudioInput : public ViAudioTransmission
{
    Q_OBJECT

	public:
		ViAudioInput(ViAudioFormat format, ViAudioBuffer *buffer = NULL);

		virtual void start() = 0;
		virtual void stop() = 0;
		virtual void pause() = 0;
		virtual void initialize() = 0;

	protected:
		virtual void free() = 0;
};

#endif
