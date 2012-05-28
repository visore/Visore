#ifndef VISTREAMINPUT_H
#define VISTREAMINPUT_H

#include <QAudioDeviceInfo>
#include "viaudioinput.h"

class ViStreamInput : public ViAudioInput
{
    Q_OBJECT

	public:

		ViStreamInput();

		virtual void setDevice(QAudioDeviceInfo device);

		virtual void start() = 0;
		virtual void stop() = 0;
		virtual void pause() = 0;

		virtual qreal volume() = 0;
		virtual void setVolume(qreal volume) = 0;
		virtual void mute(bool value) = 0;

	protected:

		QAudioDeviceInfo mDevice;
		qreal mVolume;

};

#endif
