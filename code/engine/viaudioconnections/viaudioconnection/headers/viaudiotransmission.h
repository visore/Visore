#ifndef VIAUDIOTRANSMISSION_H
#define VIAUDIOTRANSMISSION_H

#include "viaudiobuffer.h"
#include "vierror.h"
#include "viaudioformat.h"

class ViAudioTransmission : public QObject, public ViError
{
    Q_OBJECT

	public:
		enum ViTransmissionStatus
		{
			None = 0,
			Running = 1,
			Stopped = 2,
			Paused = 3
		};

		ViAudioTransmission(ViAudioFormat format, ViAudioBuffer *buffer = NULL);
		~ViAudioTransmission();
		virtual void setBuffer(ViAudioBuffer *buffer);
		virtual ViAudioBuffer* buffer();

		virtual ViAudioFormat format();
		virtual void setFormat(ViAudioFormat format);

		virtual void setStatus(ViAudioTransmission::ViTransmissionStatus status);
		virtual ViAudioTransmission::ViTransmissionStatus status();

		virtual void start() = 0;
		virtual void stop() = 0;
		virtual void pause() = 0;
		virtual void initialize() = 0;

	protected:
		virtual void free() = 0;

	protected:
		ViAudioBuffer *mBuffer;
		ViAudioBufferStream *mStream;
		ViAudioFormat mFormat;
		ViAudioTransmission::ViTransmissionStatus mStatus;
};

#endif
