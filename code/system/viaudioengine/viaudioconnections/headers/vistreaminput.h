#ifndef VISTREAMINPUT_H
#define VISTREAMINPUT_H

#include "viaudioinput.h"
#include "viaudiodevice.h"

class ViStreamInput : public ViAudioInput
{
    Q_OBJECT

	public:
		ViStreamInput(ViAudioBuffer *buffer = NULL, ViAudioMetaData *metaData = NULL, ViAudioDevice *device = NULL)
			: ViAudioInput(buffer, metaData)
		{
			mDevice = device;
		}

		~ViStreamInput()
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

	protected:
		ViAudioDevice *mDevice;
};

#endif
