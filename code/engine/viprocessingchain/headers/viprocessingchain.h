#ifndef VIPROCESSINGCHAIN_H
#define VIPROCESSINGCHAIN_H

#include <QQueue>
#include <QThread>
#include "viaudiobuffer.h"
#include "viaudioconnection.h"
#include "viprocessorlist.h"

class ViProcessingThread : public QThread
{

	public:

		ViProcessingThread();
		void setWindowSize(int windowSize);
		void setStream(ViAudioConnection::Direction direction, ViAudioBufferStream *stream);
		bool attach(ViAudioConnection::Direction direction, ViProcessor *processor);
		void run();

	protected:

		void updateProcessors();
		void updateChunks();

	private:

		int (*pcmToReal)(char*, double*, int);
		int (*realToPcm)(double*, char*, int);

		int mWindowSize;

		ViAudioBufferChunk mInputChunk;
		ViSampleChunk mRealChunk;
		ViAudioBufferChunk mOutputChunk;

		ViAudioBufferStream *mReadStream;
		ViAudioBufferStream *mWriteStream;

		ViProcessorList mProcessors;

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
		bool attach(ViAudioConnection::Direction direction, ViProcessor *processor);

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
