#ifndef VIPROCESSINGCHAIN_H
#define VIPROCESSINGCHAIN_H

#include <QQueue>
#include <QThread>
#include "viaudiobuffer.h"
#include "viaudioconnection.h"
#include "viprocessor.h"

class ViProcessingThread : public QThread
{

	public:

		ViProcessingThread();
		~ViProcessingThread();
		void setStream(ViAudioConnection::Direction direction, ViAudioBufferStream *stream);
		void setSampleSize(ViAudioConnection::Direction direction, int sampleSize);
		void attach(ViAudioConnection::Direction direction, ViProcessor *processor);
		void run();

	private:

		int (*pcmToReal)(char*, double*, int);

		ViAudioBufferChunk mRawChunk;
		ViChunk<double> mRealChunk;
		ViAudioBufferStream *mReadStream;
		ViAudioBufferStream *mWriteStream;

		QList<ViRawProcessor*> mRawInputProcessors;
		QList<ViRealProcessor*> mRealInputProcessors;
		QList<ViRawProcessor*> mRawOutputProcessors;
		QList<ViRealProcessor*> mRealOutputProcessors;

};

class ViProcessingChain : public QObject, public ViError
{
    Q_OBJECT

	signals:

		void inputChanged();
		void outputChanged();

	protected slots:

		void process();

	public:

		ViProcessingChain();
		~ViProcessingChain();

		void start();
		ViAudioBuffer* buffer(ViAudioConnection::Direction direction, ViAudioConnection::Type type);
		void attach(ViAudioConnection::Direction direction, ViProcessor *processor);

		void setInput(ViAudioFormat format, QString filePath);
		void setInput(ViAudioFormat format, QAudioDeviceInfo device);
		void addOutput(ViAudioFormat format, QString filePath);
		void addOutput(ViAudioFormat format, QAudioDeviceInfo device);

	protected:

		void changeInputBuffer(ViAudioBuffer *buffer);
		void changeOutputBuffer(ViAudioBuffer *buffer);

		ViAudioBuffer* allocateBuffer(ViAudioConnection::Direction direction);
		void nextBuffer();

	private:

		ViProcessingThread mThread;

		QQueue<ViAudioBuffer*> mInputBuffers;
		ViAudioBuffer *mInputBuffer;
		ViAudioBuffer *mOutputBuffer;

		ViAudioConnection *mConnection;
		ViAudioInput *mInput;
		QList<ViAudioOutput*> mOutputs;

};

#endif
