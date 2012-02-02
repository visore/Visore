#ifndef VIAUDIOINPUT_H
#define VIAUDIOINPUT_H

#include "viaudiotransmission.h"

class ViAudioInput : public ViAudioTransmission
{
    Q_OBJECT

	signals:
		void changed(int startIndex, int size);

	public:
		ViAudioInput(ViAudioBuffer *buffer = NULL, ViAudioMetaData *metaData = NULL);

		virtual void start() = 0;
		virtual void stop() = 0;
		virtual void pause() = 0;
		virtual void initialize() = 0;

	protected:
		virtual void free() = 0;
};

#endif
