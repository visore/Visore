#ifndef VIEXECUTOR_H
#define VIEXECUTOR_H

#include "viaudiobuffer.h"
#include "viprocessor.h"
#include "viprocessorlist.h"
#include "vipcmconverter.h"

class ViExecutor : public QThread
{
    Q_OBJECT

	signals:

		void progressed(short progress);
		void finished();

	public slots:

		void execute();

	public:

		ViExecutor();
		~ViExecutor();
		void setWindowSize(int windowSize);
		void setNotify(bool notify);
		bool attach(ViAudioConnection::Direction direction, ViProcessor *processor);
		bool detach(ViProcessor *processor);
		void setBuffer(ViAudioConnection::Direction direction, ViAudioBuffer *buffer);

	protected:

		void update();
		void connect(ViProcessor *processor);
		void disconnect(ViProcessor *processor);
		void start();
		void run();
		virtual void runNormal() = 0;
		virtual void runNotify() = 0;

	protected:

		int mWindowSize;
		bool mNotify;
		ViProcessorList mProcessors;

		ViPcmConverter<qreal> mInputConverter;
		ViPcmConverter<qreal> mOutputConverter;
		ViAudioBufferStream *mReadStream;
		ViAudioBufferStream *mWriteStream;
		ViAudioFormat mInputFormat;
		ViAudioFormat mOutputFormat;

		ViRawChunk *mInputChunk;
		ViSampleChunk *mRealChunk;
		ViRawChunk *mOutputChunk;

};

#endif
