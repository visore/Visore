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

		ViAudioTransmission(ViAudioBuffer *buffer = NULL, ViAudioMetaData *metaData = NULL)
			: QObject()
		{
			mBuffer = buffer;
			mMetaData = metaData;
			mStatus = ViAudioTransmission::None;
		}

		~ViAudioTransmission()
		{
			if(mBuffer != NULL)
			{
				delete mBuffer;
				mBuffer = NULL;
			}
			if(mMetaData != NULL)
			{
				delete mMetaData;
				mMetaData = NULL;
			}
		}

		virtual void setBuffer(ViAudioBuffer *buffer)
		{
			mBuffer = buffer;
		}

		virtual ViAudioMetaData* metaData()
		{
			return mMetaData;
		}

		virtual void setMetaData(ViAudioMetaData *metaData)
		{
			mMetaData = metaData;
		}

		virtual void setStatus(ViAudioTransmission::ViTransmissionStatus status)
		{
			mStatus = status;
		}

		virtual ViAudioTransmission::ViTransmissionStatus status()
		{
			return mStatus;
		}

		virtual void start() = 0;
		virtual void stop() = 0;
		virtual void pause() = 0;

	protected:
		ViAudioBuffer *mBuffer;
		ViAudioMetaData *mMetaData;
		ViAudioTransmission::ViTransmissionStatus mStatus;
};

#endif
