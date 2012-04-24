#ifndef VICODER_H
#define VICODER_H

#include <QThread>
#include <QString>
#include <QByteArray>
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
			UnsupportedCodecError = 2,
			DecodingError = 3,
			EncodingError = 4,
			DeviceOpenError = 5,
			NoStreamError = 6,
			NoDecoderError = 7,
			NoEncoderError = 8,
			OutOfMemoryError = 9
		};

	signals:

		void stateChanged(ViCoder::State status);

	public:

		ViCoder();
		~ViCoder();

		void encode(ViAudioBuffer *buffer, QIODevice *device, ViAudioFormat *format);
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
		ViCoder::State status();
		ViCoder::Error error();

	protected:
		ViAudioBuffer *mBuffer;
		ViAudioFormat *mFormat;
		ViCoder::State mState;
		ViCoder::Error mError;
};

class ViDecodingThread : public ViCoderThread
{
	Q_OBJECT

	public:
		ViDecodingThread(QObject *parent = 0);
		void setFile(QString file);
		void setFormat(ViAudioFormat *format);
		void run();

	private:
		QString mFile;
		
};

class ViEncodingThread : public ViCoderThread
{
	Q_OBJECT

	public:
		ViEncodingThread(QObject *parent = 0);
		void setDevice(QIODevice *device);
		void run();

	private:
		QIODevice *mDevice;
};

#endif
