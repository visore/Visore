#ifndef VIEXECUTOR_H
#define VIEXECUTOR_H

#include "viaudiobuffer.h"
#include "viprocessor.h"
#include "viprocessorlist.h"
#include "vipcmconverter.h"
#include "viaudio.h"
#include <QTimer>

class ViExecutor : public QThread
{
    Q_OBJECT

	signals:

		void progressed(short progress);
		void processingRateChanged(qreal rate);

	public slots:

		void execute();
		void initialize();
		void finalize();

	protected slots:

		void updateProcessingRate();

	public:

		ViExecutor();
		~ViExecutor();
		int processingRate();
		void setWindowSize(int windowSize);
		void setNotify(bool notify);
		bool attach(ViAudio::Mode mode, ViProcessor *processor);
		bool detach(ViProcessor *processor);
		void setBuffer(ViAudio::Mode mode, ViAudioBuffer *buffer);

		static int defaultWindowSize();

	protected:

		void connect(ViProcessor *processor);
		void disconnect(ViProcessor *processor);
		void start();
		void run();
		virtual void runNormal() = 0;
		virtual void runNotify() = 0;

	protected:

		QTimer mTimer;
		qreal mProcessingRate;
		qint64 mProcessedSamples;
		qint64 mRateCounter;

		int mWindowSize;
		bool mNotify;
		bool mWasInitialized;
		ViProcessorList mProcessors;

		ViPcmConverter<qreal> mInputConverter;
		ViPcmConverter<qreal> mOutputConverter;
		ViAudioBufferStream *mReadStream;
		ViAudioBufferStream *mWriteStream;
		ViAudioFormat mInputFormat;
		ViAudioFormat mOutputFormat;

		ViRawChunk *mInputChunk;
		ViSampleChunk *mInputSamples;
		ViSampleChunk *mOutputSamples;
		ViRawChunk *mOutputChunk;

};

#endif
