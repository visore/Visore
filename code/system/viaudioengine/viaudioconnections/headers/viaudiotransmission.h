#ifndef VIAUDIOTRANSMISSION_H
#define VIAUDIOTRANSMISSION_H

#include "viaudiobuffer.h"
#include "viaudiometadata.h"

class ViAudioTransmission : public QObject, public ViError
{
    Q_OBJECT

	public:
		ViAudioTransmission(ViAudioBuffer *buffer = NULL, ViAudioMetaData *metaData = NULL)
			: QObject()
		{
			mBuffer = buffer;
			mMetaData = metaData;
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

		virtual void start() = 0;

	protected:
		ViAudioBuffer *mBuffer;
		ViAudioMetaData *mMetaData;
};

#endif
