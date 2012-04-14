#ifndef VICODECS_H
#define VICODECS_H

#include <QThread>
#include <QString>
#include <QByteArray>
#include "visingleton.h"
#include "viaudiobuffer.h"

class ViEncodingThread;

class ViCodecs : public ViSingleton
{
	public:
		enum Status
		{
			Uninitialized = 0,
			Started = 1,
			Success = 2,
			Failure = 3
		};
		enum Error
		{
			None = 0,
			Initialization = 1,
			UnsupportedCodec = 2,
			Decoding = 3,
			FileOpen = 4,
			NoStream = 5,
			NoDecoder = 6
		};
		~ViCodecs();
		static ViCodecs* instance();
		static ViCodecs::Status encode(QByteArray *inData, QByteArray *outData);
		static void decode(QString file, ViAudioBuffer *buffer);

	protected:
		ViCodecs();
		
	protected:
		static QSharedPointer<ViCodecs> mInstance;

	private:
		ViEncodingThread *mEncodingThread;
};

class ViCodecsThread : public QThread
{
	public:
		ViCodecsThread(QObject *parent = 0);
		ViCodecs::Status status();
		ViCodecs::Error error();

	protected:
		ViCodecs::Status mStatus;
		ViCodecs::Error mError;
};

class ViEncodingThread : public ViCodecsThread
{
	public:
		ViEncodingThread(QObject *parent = 0);
		void setFile(QString file);
		void setBuffer(ViAudioBuffer *buffer);
		void run();

	private:
		QString mFile;
		ViAudioBuffer *mBuffer;
};

#endif
