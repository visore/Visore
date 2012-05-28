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

	private:

		void changeInput(); // Connect song end detector to this slot
		void finish();

	public:

		ViProcessingChain();
		~ViProcessingChain();

		void setWindowSize(int windowSize);
		void setTransmission(ViAudioTransmission *transmission);
		bool attach(ViAudioConnection::Direction direction, ViProcessor *processor);
		bool detach(ViProcessor *processor);
		ViAudioBuffer* buffer(ViAudioConnection::Direction direction);

	protected:

		void allocateBuffer(ViAudioConnection::Direction direction);
		void nextBuffer();

	private:

		ViMultiExecutor mMultiExecutor;
		ViAudioInput *mInput;
		ViAudioOutput *mOutput;

		QQueue<ViAudioBuffer*> mInputBuffers;
		ViAudioBuffer *mInputBuffer;
		ViAudioBuffer *mOutputBuffer;	

};

#endif
