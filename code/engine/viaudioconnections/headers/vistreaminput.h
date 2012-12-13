#ifndef VISTREAMINPUT_H
#define VISTREAMINPUT_H

#include <QBuffer>
#include <QTimer>
#include <QAudioInput>
#include <QAudioDeviceInfo>
#include "viaudioinput.h"

class ViStreamBuffer : public QBuffer
{

	Q_OBJECT

	private slots:

		void addData(qint64 bytes);

	public:

		ViStreamBuffer(ViBuffer *audioBuffer);
		~ViStreamBuffer();
		void clear();

	private:

		QDataStream *mReadStream;
		ViBufferStreamPointer mWriteStream;

};

class ViStreamInput : public ViAudioInput
{
    Q_OBJECT

	private slots:

		void checkBuffer();

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

	private:

		void startChecking();
		void stopChecking();

	private:

		QAudioDeviceInfo mDevice;
		qreal mVolume;
		QAudioInput *mAudioInput;
		ViStreamBuffer *mBufferDevice;
		qreal mMuteVolume;
		bool mIsMute;
		ViAudioFormat mFormat;

		QTimer mTimer;
		qint64 mPreviousSize;

};

#endif
