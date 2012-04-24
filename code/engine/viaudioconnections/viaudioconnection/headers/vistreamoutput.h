#ifndef VISTREAMOUTPUT_H
#define VISTREAMOUTPUT_H

#include <QAudioDeviceInfo>
#include "viaudiooutput.h"

class ViStreamOutput : public ViAudioOutput
{
    Q_OBJECT

	signals:

		void positionChanged(ViAudioPosition position);

	public slots:

		virtual void bufferChanged(int size);

	public:

		ViStreamOutput();

		virtual void initialize();
		virtual void free();

		virtual void setDevice(QAudioDeviceInfo device);

		virtual void start() = 0;
		virtual void stop() = 0;
		virtual void pause() = 0;

		virtual bool setPosition(ViAudioPosition position) = 0; //returns negative number if position is invalid
		virtual ViAudioPosition position() = 0;

		virtual qreal volume() = 0;
		virtual void setVolume(qreal volume) = 0;
		virtual void mute(bool value) = 0;

	protected:

		QAudioDeviceInfo mDevice;
		qreal mVolume;
};

#endif
