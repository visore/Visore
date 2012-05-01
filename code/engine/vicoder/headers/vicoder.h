#ifndef VICODER_H
#define VICODER_H

#include <QThread>
#include <QString>
#include <QByteArray>
#include <QFile>
#include "viaudiobuffer.h"
#include "viobject.h"
#include "viaudioformat.h"
#include "vicodecmanager.h"

class ViEncodingThread;
class ViDecodingThread;

class ViCoder : public QObject
{
	Q_OBJECT

	public:

		enum State
		{
			UninitializedState = 0,
			ActiveState = 1,
			SuccessState = 2,
			FailureState = 3
		};
		enum Error
		{
			NoError = 0,
			InitializationError = 1,
			UnsupportedFormatError = 2,
			UnsupportedCodecError = 3,
			DecodingError = 4,
			EncodingError = 5,
			DeviceError = 6,
			NoStreamError = 7,
			NoDecoderError = 8,
			NoEncoderError = 9,
			OutOfMemoryError = 10
		};

	signals:

		void stateChanged(ViCoder::State status);

	public:

		ViCoder();
		~ViCoder();

		void encode(ViAudioBuffer *buffer, QFile *file, ViAudioFormat *format);
		void decode(QString file, ViAudioBuffer *buffer, ViAudioFormat *format);
		void stop();
	
	private:

		ViEncodingThread *mEncodingThread;
		ViDecodingThread *mDecodingThread;
};

class ViCoderThread : public QThread
{
	Q_OBJECT

	signals:
		void stateChanged(ViCoder::State status);

	public:
		ViCoderThread(QObject *parent = 0);
		virtual void setFormat(ViAudioFormat *format);
		virtual void setBuffer(ViAudioBuffer *buffer);
		virtual void setFile(QString file);
		ViCoder::State status();
		ViCoder::Error error();

	protected:
		ViAudioBuffer *mBuffer;
		ViAudioFormat *mFormat;
		ViCoder::State mState;
		ViCoder::Error mError;
		QString mFile;
};

class ViDecodingThread : public ViCoderThread
{
	Q_OBJECT

	public:
		ViDecodingThread(QObject *parent = 0);
		void setFormat(ViAudioFormat *format);
		void run();	
};

class ViEncodingThread : public ViCoderThread
{
	Q_OBJECT

	public:
		ViEncodingThread(QObject *parent = 0);
		~ViEncodingThread();
		void setDevice(QIODevice *device);
		void setFormat(ViAudioFormat *format);
		void run();

	private:
		QIODevice *mDevice;
};

#endif
