#ifndef VIPROCESSINGCHAIN_H
#define VIPROCESSINGCHAIN_H

#include <QQueue>
#include <QTime>
#include <QTimer>
#include "viaudioconnection.h"
#include "vimultiexecutor.h"

class ViProcessingChain : public QObject
{
    Q_OBJECT

	signals:

		void changed();
		void buffering(short progress);

	private slots:

		void changeInput(ViAudioPosition position); // Connect song end detector to this slot
		void finish();
		void handleUnderrun();
		void updateBuffering(qreal processingRate);

	public:

		ViProcessingChain();
		~ViProcessingChain();

		void setWindowSize(int windowSize);
		void setTransmission(ViAudioTransmission *transmission);
		bool attach(ViAudio::Mode mode, ViProcessor *processor);
		bool detach(ViProcessor *processor);
		ViAudioBuffer* buffer(ViAudio::Mode mode);

	protected:

		ViAudioBuffer* allocateBuffer(ViAudio::Mode mode);
		void nextBuffer();

	private:

		bool mEndDetected;
		ViMultiExecutor mMultiExecutor;
		ViAudioInput *mInput;
		ViStreamOutput *mOutput;

		QQueue<ViAudioBuffer*> mInputBuffers;
		ViAudioBuffer *mInputBuffer;
		ViAudioBuffer *mOutputBuffer;

		int mSecondsPassed;
		int mSecondsNeeded;
};

#endif
