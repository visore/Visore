#ifndef VISTREAMINPUT_H
#define VISTREAMINPUT_H

#include <QBuffer>
#include <QAudioInput>
#include <QAudioDeviceInfo>
#include "viaudioinput.h"

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

};

#endif
