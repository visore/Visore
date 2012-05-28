#ifndef VIAUDIOINPUT_H
#define VIAUDIOINPUT_H

#include "viaudiotransmission.h"

class ViAudioInput : public ViAudioTransmission
{
    Q_OBJECT

	signals:

		void formatChanged(ViAudioFormat format);

	public:

		ViAudioInput();
		
		virtual void setBuffer(ViAudioBuffer *buffer);

		virtual void start() = 0;
		virtual void stop() = 0;
		virtual void pause() = 0;
};

#endif
