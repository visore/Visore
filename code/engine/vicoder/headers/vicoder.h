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
			FileOpenError = 4,
			NoStreamError = 5,
			NoDecoderError = 6
		};

	signals:

		void stateChanged(ViCoder::State status);

	public:

		ViCoder();
		~ViCoder();
		ViCoder::State encode(QByteArray *inData, QByteArray *outData, ViAudioFormat *format);
		void decode(QString file, ViAudioBuffer *buffer, ViAudioFormat *format);
	
	private:
		ViEncodingThread *mEncodingThread;
};

class ViCoderThread : public QThread
{
	Q_OBJECT

	signals:
		void stateChanged(ViCoder::State status);

	public:
		ViCoderThread(QObject *parent = 0);
		virtual void setFormat(ViAudioFormat *format);
		ViCoder::State status();
		ViCoder::Error error();

	protected:
		ViAudioFormat *mFormat;
		ViCoder::State mState;
		ViCoder::Error mError;
};

class ViEncodingThread : public ViCoderThread
{
	Q_OBJECT

	public:
		ViEncodingThread(QObject *parent = 0);
		void setFile(QString file);
		void setBuffer(ViAudioBuffer *buffer);
		void setFormat(ViAudioFormat *format);
		void run();

	private:
		QString mFile;
		ViAudioBuffer *mBuffer;
};

#endif
