#ifndef VIAUDIOOUTPUT_H
#define VIAUDIOOUTPUT_H

#include "viaudiotransmission.h"

class ViAudioOutput : public ViAudioTransmission
{
    Q_OBJECT

	signals:
		void positionChanged(qint64 bytes, qint64 milliseconds, qint8 bits);

	public slots:
		virtual void bufferChanged(int size) = 0;

	public:
		ViAudioOutput(ViAudioBuffer *buffer = NULL, ViAudioMetaData *metaData = NULL);

		virtual void start() = 0;
		virtual void stop() = 0;
		virtual void pause() = 0;
		virtual void initialize() = 0;

		virtual qint64 setPosition(ViAudioTransmission::ViTransmissionType type, qint64 position) = 0; //returns negative number if position is invalid
		virtual qint64 position(ViAudioTransmission::ViTransmissionType type) = 0;

	protected:
		virtual void free() = 0;
};

#endif
