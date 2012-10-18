#ifndef VIEXECUTOR_H
#define VIEXECUTOR_H

#include "vibuffer.h"
#include "viprocessor.h"
#include "viprocessorlist.h"
#include "vipcmconverter.h"
#include "viaudio.h"
#include "viaudioobject.h"
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
		void setFormat(ViAudioFormat format);

	public:

		ViExecutor();
		~ViExecutor();
		qreal processingRate();
		void setWindowSize(int windowSize);
		void setNotify(bool notify);
		bool attach(ViAudio::Mode mode, ViProcessor *processor);
		bool detach(ViProcessor *processor);
		void setObject(ViAudioObjectPointer object);

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

		ViAudioObjectPointer mObject;
		ViBufferStreamPointer mReadStream;
		ViBufferStreamPointer mWriteStream;

		ViAudioFormat mInputFormat;
		ViAudioFormat mOutputFormat;

		ViRawChunk *mInputChunk;
		ViSampleChunk *mInputSamples;
		ViSampleChunk *mOutputSamples;
		ViRawChunk *mOutputChunk;

};

#endif
