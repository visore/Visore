#ifndef VISTREAMOUTPUT_H
#define VISTREAMOUTPUT_H

#include "viaudiooutput.h"
#include "viaudiodevice.h"

class ViStreamOutput : public ViAudioOutput
{
    Q_OBJECT

	public:
		ViStreamOutput(ViAudioBuffer *buffer = NULL, ViAudioMetaData* metaData = NULL, ViAudioDevice *device = NULL)
			: ViAudioOutput(buffer, metaData)
		{
			mDevice = device;
		}

		~ViStreamOutput()
		{
			if(mDevice != NULL)
			{
				delete mDevice;
				mDevice = NULL;
			}
		}

		virtual void setDevice(ViAudioDevice *device)
		{
			mDevice = device;
		}

		virtual void start() = 0;
		virtual void stop() = 0;
		virtual void pause() = 0;

	protected:
		ViAudioDevice *mDevice;
};

#endif
