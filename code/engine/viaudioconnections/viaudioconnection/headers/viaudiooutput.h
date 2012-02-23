#ifndef VIAUDIOOUTPUT_H
#define VIAUDIOOUTPUT_H

#include "viaudiotransmission.h"
#include "viaudioposition.h"

class ViAudioOutput : public ViAudioTransmission
{
    Q_OBJECT

	signals:
		void positionChanged(ViAudioPosition position);

	public slots:
		virtual void bufferChanged(int size) = 0;

	public:
		ViAudioOutput(ViAudioBuffer *buffer = NULL, ViAudioMetaData *metaData = NULL);

		virtual void start() = 0;
		virtual void stop() = 0;
		virtual void pause() = 0;
		virtual void initialize() = 0;

		virtual bool setPosition(ViAudioPosition position) = 0; //returns negative number if position is invalid
		virtual ViAudioPosition position() = 0;

	protected:
		virtual void free() = 0;
};

#endif
