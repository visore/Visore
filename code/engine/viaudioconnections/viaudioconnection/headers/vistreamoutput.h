#ifndef VISTREAMOUTPUT_H
#define VISTREAMOUTPUT_H

#include "viaudiooutput.h"
#include "viaudiodevice.h"

class ViStreamOutput : public ViAudioOutput
{
    Q_OBJECT

	public slots:
		virtual void bufferChanged(int size);

	public:
		ViStreamOutput(ViAudioBuffer *buffer = NULL, ViAudioMetaData* metaData = NULL, ViAudioDevice *device = NULL);
		~ViStreamOutput();
		virtual void setDevice(ViAudioDevice *device);

		virtual void start() = 0;
		virtual void stop() = 0;
		virtual void pause() = 0;
		virtual void initialize() = 0;

		virtual qint64 setPosition(ViAudioTransmission::ViTransmissionType type, qint64 position) = 0; //returns negative number if position is invalid
		virtual qint64 position(ViAudioTransmission::ViTransmissionType type) = 0;

	protected:
		virtual void free() = 0;

	protected:
		ViAudioDevice *mDevice;
};

#endif
