#ifndef VIPROCESSORTHREAD_H
#define VIPROCESSORTHREAD_H

#include <QThread>
#include "viaudiobufferstream.h"
#include "viaudioconnection.h"
#include "viprocessorlist.h"

class ViProcessorThread : public QThread
{
    Q_OBJECT

	signals:

		void progressed(short progress);
		void finished();

	public:

		enum Execution
		{
			Observation = 0,
			Modification = 1,
			ObservationModification = 2
		};

		ViProcessorThread(ViProcessorThread::Execution execution = ViProcessorThread::ObservationModification, bool notify = false);
		
		void setNotification(bool notify);
		void setExecution(ViProcessorThread::Execution execution);
		void setWindowSize(int windowSize);
		void setStream(ViAudioConnection::Direction direction, ViAudioBufferStream *stream);
		bool attach(ViAudioConnection::Direction direction, ViProcessor *processor);

		void reset();
		void run();

	protected:

		void normalExecute();
		void notifyExecute();

		void observationProcess();
		void modificationProcess();
		void observationModificationProcess();

		void updateProcessors();
		void updateChunks();

	private:

		void (ViProcessorThread::*execute)();
		void (ViProcessorThread::*process)();
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

#endif
