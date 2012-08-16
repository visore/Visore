#ifndef VIPROCESSINGCHAIN_H
#define VIPROCESSINGCHAIN_H

#include <QQueue>
#include <QTime>
#include <QTimer>
#include "viaudioconnection.h"
#include "vimultiexecutor.h"
#include "viprojectmanager.h"

class ViHandler;

class ViProcessingChain : public QObject
{
    Q_OBJECT

	signals:

		void changed();
		void progress(short progress);
		void statusChanged(QString status);

		void streamOutputChanged(ViStreamOutput*);

	private slots:

		void changeInput(ViAudioPosition position); // Connect song end detector to this slot
		void startInput(ViAudioPosition position);
		void finalize();
		void finishWriting();
		void finishPlaying();

	public:

		ViProcessingChain();
		~ViProcessingChain();

		void setWindowSize(int windowSize);
		void setTransmission(ViAudioTransmission *transmission);
		void setProject(ViProject *project, ViAudioFormat format);
		bool attach(ViAudio::Mode mode, ViProcessor *processor);
		bool detach(ViProcessor *processor);
		ViAudioBuffer* buffer(ViAudio::Mode mode);

		ViStreamOutput* streamOutput();
		ViExecutor* executor();

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

		ViProject *mProject;

		QList<ViHandler*> mHandlers;
};

#endif
