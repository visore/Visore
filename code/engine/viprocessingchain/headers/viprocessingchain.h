#ifndef VIPROCESSINGCHAIN_H
#define VIPROCESSINGCHAIN_H

#include <QQueue>
#include <QTime>
#include <QTimer>
#include "viaudioconnection.h"
#include "vimultiexecutor.h"
#include "viproject.h"

class ViProcessingChain : public QObject
{
    Q_OBJECT

	signals:

		void changed();
		void buffering(short progress);

	private slots:

		void changeInput(ViAudioPosition position); // Connect song end detector to this slot
		void finalize();
		void finish();
		void handleUnderrun();
		void updateBuffering(qreal processingRate);

	public:

		ViProcessingChain();
		~ViProcessingChain();

		void setWindowSize(int windowSize);
		void setTransmission(ViAudioTransmission *transmission);
		void setProject(QString filePath);
		bool attach(ViAudio::Mode mode, ViProcessor *processor);
		bool detach(ViProcessor *processor);
		ViAudioBuffer* buffer(ViAudio::Mode mode);

	protected:

		ViAudioBuffer* allocateBuffer(ViAudio::Mode mode);
		void nextBuffer(ViAudio::Mode mode);

	private:

		ViMultiExecutor mMultiExecutor;
		ViAudioInput *mInput;
		ViStreamOutput *mStreamOutput;
		ViFileOutput *mFileOutput;

		QQueue<ViAudioBuffer*> mInputBuffers;
		QQueue<ViAudioBuffer*> mOutputBuffers;
		ViAudioBuffer *mInputBuffer;
		ViAudioBuffer *mOutputBuffer;

		int mSecondsPassed;
		int mSecondsNeeded;

		ViProject *mProject;
};

#endif
