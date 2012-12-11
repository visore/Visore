#ifndef VISTREAMINPUT_H
#define VISTREAMINPUT_H

#include <QBuffer>
#include <QAudioInput>
#include <QAudioDeviceInfo>
#include "viaudioinput.h"
#include <QTimer>
class ViStreamBuffer : public QBuffer
{

	public:

		ViStreamBuffer();
		void setBuffer(ViBuffer *buffer);
		qint64 write(const char *data, qint64 maxSize);
		qint64 write(const char *data);
		qint64 write(const QByteArray &byteArray);
		qint64 writeData(const char *data, qint64 length);

	private:

		ViBufferStreamPointer mStream;

};

class ViStreamInput : public ViAudioInput
{
    Q_OBJECT

private slots:

void tu()
{
	LOG("SI error: "+QString::number(mAudioInput->error()));
	LOG("SI status: "+QString::number(mAudioInput->state()));
	LOG("SI buffer: "+QString::number(mBuffer->size()));
}

	public:

		ViStreamInput();
		~ViStreamInput();

		void setDevice(QAudioDeviceInfo device);
		void setBuffer(ViBuffer *buffer);
		ViAudioFormat format();
		void setFormat(ViAudioFormat format);

		void start();
		void stop();
		void pause();

		qreal volume();
		void setVolume(qreal volume);
		void mute(bool value);

	protected:

		QAudioDeviceInfo mDevice;
		qreal mVolume;
		QAudioInput *mAudioInput;
		ViStreamBuffer mBufferDevice;
		ViAudioPosition mOldPosition;
		qreal mMuteVolume;
		bool mIsMute;
		ViAudioFormat mFormat;
QTimer *timer;

};

#endif
