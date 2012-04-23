#ifndef VISTREAMINPUT_H
#define VISTREAMINPUT_H

#include "viaudioinput.h"
#include "viaudiodevice.h"

class ViStreamInput : public ViAudioInput
{
    Q_OBJECT

	public:
		ViStreamInput(ViAudioFormat format, ViAudioBuffer *buffer = NULL, ViAudioDevice *device = NULL);
		~ViStreamInput();
		virtual void setDevice(ViAudioDevice *device);

		virtual void start() = 0;
		virtual void stop() = 0;
		virtual void pause() = 0;
		virtual void initialize() = 0;

	protected:
		virtual void free() = 0;

	protected:
		ViAudioDevice *mDevice;
};

#endif
