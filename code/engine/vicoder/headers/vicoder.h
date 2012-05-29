#ifndef VICODER_H
#define VICODER_H

#include <QThread>
#include <QString>
#include <QByteArray>
#include <QFile>
#include "viaudiobuffer.h"
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
			IdleState = 0,
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
			OutOfMemoryError = 10,
			ResampleError = 11
		};

	signals:

		void stateChanged(ViCoder::State status);

	public:

		ViCoder();
		~ViCoder();

		void encode(ViAudioBuffer *buffer, QString filePath, ViAudioFormat inputFormat, ViAudioFormat outputFormat);
		void encode(ViAudioBuffer *buffer, QByteArray *outputBuffer, ViAudioFormat inputFormat, ViAudioFormat outputFormat);
		void decode(QString file, ViAudioBuffer *buffer, ViAudioFormat *format);
		void stop();
		ViCoder::Error error();
	
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
		virtual void setInputFormat(ViAudioFormat *format);
		virtual void setOutputFormat(ViAudioFormat *format);
		virtual void setBuffer(ViAudioBuffer *buffer);
		virtual void setFile(QString file);

		ViCoder::State state();
		ViCoder::Error error();

	protected:
		ViAudioBuffer *mBuffer;
		ViAudioFormat *mInputFormat;
		ViAudioFormat *mOutputFormat;
		ViCoder::State mState;
		ViCoder::Error mError;
		QString mFile;
};

class ViDecodingThread : public ViCoderThread
{
	Q_OBJECT

	public:
		ViDecodingThread(QObject *parent = 0);
		void setOutputFormat(ViAudioFormat *format);
		void run();	
};

class ViEncodingThread : public ViCoderThread
{
	Q_OBJECT

	public:
		ViEncodingThread(QObject *parent = 0);
		~ViEncodingThread();
		void setInputFormat(ViAudioFormat *format);
		void setOutputFormat(ViAudioFormat *format);
		void setOutputBuffer(QByteArray *outputBuffer);
		void run();

	private:
		QByteArray *mOutputBuffer;
};

#endif
