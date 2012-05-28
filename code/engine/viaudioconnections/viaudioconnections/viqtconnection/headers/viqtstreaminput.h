#ifndef VIQTSTREAMINPUT_H
#define VIQTSTREAMINPUT_H

#include "vistreaminput.h"
#include <QAudioInput>
#include <QBuffer>

class ViQtStreamBuffer : public QBuffer
{

	public:

		ViQtStreamBuffer();
		void setBuffer(ViAudioBuffer *buffer);
		qint64 write(const char *data, qint64 maxSize);
		qint64 write(const char *data);
		qint64 write(const QByteArray &byteArray);
		qint64 writeData(const char *data, qint64 length);

	private:

		ViAudioBufferStream *mStream;

};

class ViQtStreamInput : public ViStreamInput
{
	Q_OBJECT

	public:

		ViQtStreamInput();
		~ViQtStreamInput();

		void setBuffer(ViAudioBuffer *buffer);
		void setFormat(ViAudioFormat format);

		void start();
		void stop();
		void pause();

		qreal volume();
		void setVolume(qreal volumeValue);
		void mute(bool value);
		
	private:

		QAudioInput *mAudioInput;
		ViQtStreamBuffer mBufferDevice;
		ViAudioPosition mOldPosition;

		qreal mMuteVolume;
		bool mIsMute;

};

#endif
