#ifndef VIAUDIOOUTPUT_H
#define VIAUDIOOUTPUT_H

#include "viaudiotransmission.h"

class ViAudioOutput : public ViAudioTransmission
{
    Q_OBJECT

	private slots:
		virtual void changeReceived(int startIndex, int size) = 0;

	public:
		ViAudioOutput(ViAudioBuffer *buffer = NULL, ViAudioMetaData *metaData = NULL)
			: ViAudioTransmission(buffer, metaData)
		{
		}

		virtual void start() = 0;
};

#endif
