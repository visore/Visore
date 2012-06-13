#ifndef VIPROCESSINGCHAIN_H
#define VIPROCESSINGCHAIN_H

#include <QQueue>
#include "viaudioconnection.h"
#include "vimultiexecutor.h"

class ViProcessingChain : public QObject
{
    Q_OBJECT

	signals:

		void changed();

	private slots:

		void changeInput(); // Connect song end detector to this slot
		void finish();

	public:

		ViProcessingChain();
		~ViProcessingChain();

		void setWindowSize(int windowSize);
		void setTransmission(ViAudioTransmission *transmission);
		bool attach(ViAudio::Mode mode, ViProcessor *processor);
		bool detach(ViProcessor *processor);
		ViAudioBuffer* buffer(ViAudio::Mode mode);

	protected:

		void allocateBuffer(ViAudio::Mode mode);
		void nextBuffer();

	private:

		bool mEndDetected;
		ViMultiExecutor mMultiExecutor;
		ViAudioInput *mInput;
		ViAudioOutput *mOutput;

		QQueue<ViAudioBuffer*> mInputBuffers;
		ViAudioBuffer *mInputBuffer;
		ViAudioBuffer *mOutputBuffer;	

};

#endif
