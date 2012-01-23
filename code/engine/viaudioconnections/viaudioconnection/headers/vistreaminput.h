#ifndef VISTREAMINPUT_H
#define VISTREAMINPUT_H

#include "viaudioinput.h"
#include "viaudiodevice.h"

class ViStreamInput : public ViAudioInput
{
    Q_OBJECT

	public:
		ViStreamInput(ViAudioBuffer *buffer = NULL, ViAudioMetaData *metaData = NULL, ViAudioDevice *device = NULL);
		~ViStreamInput();
		virtual void setDevice(ViAudioDevice *device);

		virtual void start() = 0;
		virtual void stop() = 0;
		virtual void pause() = 0;

	protected:
		ViAudioDevice *mDevice;
};

#endif
