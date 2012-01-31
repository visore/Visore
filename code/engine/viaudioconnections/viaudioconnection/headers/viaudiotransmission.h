#ifndef VIAUDIOTRANSMISSION_H
#define VIAUDIOTRANSMISSION_H

#include "viaudiobuffer.h"
#include "viaudiometadata.h"

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
		enum ViTransmissionType
		{
			Bytes = 0,
			Seconds = 1,
			Milliseconds = 2
		};

		ViAudioTransmission(ViAudioBuffer *buffer = NULL, ViAudioMetaData *metaData = NULL);
		~ViAudioTransmission();
		virtual void setBuffer(ViAudioBuffer *buffer);
		virtual ViAudioMetaData* metaData();
		virtual void setMetaData(ViAudioMetaData *metaData);
		virtual void setStatus(ViAudioTransmission::ViTransmissionStatus status);
		virtual ViAudioTransmission::ViTransmissionStatus status();

		virtual void start() = 0;
		virtual void stop() = 0;
		virtual void pause() = 0;

	protected:
		ViAudioBuffer *mBuffer;
		ViAudioBufferStream *mStream;
		ViAudioMetaData *mMetaData;
		ViAudioTransmission::ViTransmissionStatus mStatus;
};

#endif
