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
		ViStreamOutput(ViAudioFormat format, ViAudioBuffer *buffer = NULL, ViAudioDevice *device = NULL);
		~ViStreamOutput();
		virtual void setDevice(ViAudioDevice *device);

		virtual void start() = 0;
		virtual void stop() = 0;
		virtual void pause() = 0;
		virtual void initialize() = 0;

		virtual bool setPosition(ViAudioPosition position) = 0; //returns negative number if position is invalid
		virtual ViAudioPosition position() = 0;

		virtual void mute(bool value) = 0;
		virtual qreal volume() = 0;
		virtual void setVolume(qreal volume) = 0;

	protected:
		virtual void free() = 0;

	protected:
		ViAudioDevice *mDevice;
		qint16 mVolume;
};

#endif
