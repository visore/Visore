#ifndef VISTREAMOUTPUT_H
#define VISTREAMOUTPUT_H

#include <QBuffer>
#include <QAudioOutput>
#include <QAudioDeviceInfo>
#include "viaudiooutput.h"

class ViStreamOutput : public ViAudioOutput
{
    Q_OBJECT

	signals:

		void positionChanged(ViAudioPosition position);

	private slots:

		void checkPosition();

	public:

		ViStreamOutput();
		~ViStreamOutput();

		void setDevice(QAudioDeviceInfo device);
		void setBuffer(ViAudioBuffer *buffer);
		void setFormat(ViAudioFormat format);

		void start();
		void stop();
		void pause();

		bool setPosition(ViAudioPosition position); //returns negative number if position is invalid
		ViAudioPosition position();

		qreal volume();
		void setVolume(qreal volume);
		void mute(bool value);

	protected:

		QAudioDeviceInfo mDevice;
		qreal mVolume;
		QAudioOutput *mAudioOutput;
		QBuffer mBufferDevice;
		ViAudioPosition mOldPosition;
		qreal mMuteVolume;
		bool mIsMute;
};

#endif
