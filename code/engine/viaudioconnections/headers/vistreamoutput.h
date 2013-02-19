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
		void lengthChanged(ViAudioPosition length);
		void underrun();

		void volumeChanged(int volume);
		void muted();
		void unmuted();

	public slots:

		bool setPosition(ViAudioPosition position); //returns negative number if position is invalid
		bool setPosition(int seconds);

	private slots:

		void checkPosition();
		void checkLength();
		void checkUnderrun();

	public:

		ViStreamOutput();
		~ViStreamOutput();

		void setDevice(QAudioDeviceInfo device);
		void setBuffer(ViBuffer *buffer);
		ViAudioFormat format();

		void start();
		void stop();
		void pause();

		ViAudioPosition position();

		qreal volume();
		void setVolume(qreal volume);
		void mute(bool value = true);
		void unmute();

	protected:

		QAudioDeviceInfo mDevice;
		qreal mVolume;
		QAudioOutput *mAudioOutput;
		QBuffer mBufferDevice;
		ViAudioPosition mOldPosition;
		qint64 mOldLength;
		qreal mMuteVolume;
		bool mIsMute;
};

#endif
