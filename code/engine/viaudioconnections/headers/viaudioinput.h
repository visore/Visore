#ifndef VIAUDIOINPUT_H
#define VIAUDIOINPUT_H

#include "viaudiotransmission.h"

class ViAudioInput : public ViAudioTransmission
{
    Q_OBJECT

	signals:
		void changed(int startIndex, int size);

	public:
		ViAudioInput(ViAudioBuffer *buffer = NULL, ViAudioMetaData *metaData = NULL)
			: ViAudioTransmission(buffer, metaData)
		{
		}

		virtual void start() = 0;
		virtual void stop() = 0;
		virtual void pause() = 0;
};

#endif
