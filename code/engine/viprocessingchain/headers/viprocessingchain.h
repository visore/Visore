#ifndef VIPROCESSINGCHAIN_H
#define VIPROCESSINGCHAIN_H

#include <QQueue>
#include <QThread>
#include "viaudiobuffer.h"
#include "viaudioconnection.h"
#include "viprocessorlist.h"
#include "viprocessorthread.h"

class ViProcessingChain : public QObject
{
    Q_OBJECT

	signals:

		void progressed(short progress);
		void finished();
		void inputChanged();
		void outputChanged();

	protected slots:

		void process();

	public:

		ViProcessingChain();
		~ViProcessingChain();

		void setWindowSize(int windowSize);
		void start();
		ViAudioBuffer* buffer(ViAudioConnection::Direction direction);

		bool attach(ViAudioConnection::Direction direction, ViProcessor *processor);
		//bool detach(ViProcessor *processor);

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

		ViProcessorThread mThread;

		QQueue<ViAudioBuffer*> mInputBuffers;
		ViAudioBuffer *mInputBuffer;
		ViAudioBuffer *mOutputBuffer;

		ViAudioConnection *mConnection;
		ViAudioInput *mInput;
		QList<ViAudioOutput*> mOutputs;

};

#endif
