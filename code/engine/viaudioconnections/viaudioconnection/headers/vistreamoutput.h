#ifndef VISTREAMOUTPUT_H
#define VISTREAMOUTPUT_H

#include "viaudiooutput.h"
#include "viaudiodevice.h"

class ViStreamOutput : public ViAudioOutput
{
    Q_OBJECT

	public:
		ViStreamOutput(ViAudioBuffer *buffer = NULL, ViAudioMetaData* metaData = NULL, ViAudioDevice *device = NULL);
		~ViStreamOutput();
		virtual void setDevice(ViAudioDevice *device);

		virtual void start() = 0;
		virtual void stop() = 0;
		virtual void pause() = 0;

	protected:
		ViAudioDevice *mDevice;
};

#endif
