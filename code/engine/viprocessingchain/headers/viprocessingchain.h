#ifndef VIPROCESSINGCHAIN_H
#define VIPROCESSINGCHAIN_H

#include <QQueue>
#include <QTime>
#include <QTimer>
#include "viaudioconnection.h"
#include "vimultiexecutor.h"
#include "viprojectmanager.h"

class ViHandler;
class ViUnderrunHandler;
class ViProjectHandler;
class ViSectionHandler;
class ViPlaybackHandler;

class ViProcessingChain : public QObject
{
    Q_OBJECT

	friend class ViHandler;
	friend class ViUnderrunHandler;
	friend class ViProjectHandler;
	friend class ViSectionHandler;
	friend class ViPlaybackHandler;

	signals:

		void changed();
		void progress(short progress);
		void statusChanged(QString status);

		void inputChanged();
		void outputChanged();
		void inputStarted();
		void outputStarted();
		void finishedProcessing();

		void attached(ViProcessor *processor);

		void songStarted();
		void songEnded();
		void recordStarted();
		void recordEnded();

	private slots:

		void startInput();
		void endInput();
		void startOutput();
		void endOutput();

		void finishProcessing();

	public:

		ViProcessingChain();
		~ViProcessingChain();

		void setWindowSize(int windowSize);
		void setTransmission(ViAudioTransmission *transmission);
		void setProject(ViProject *project, ViAudioFormat format);
		bool attach(ViAudio::Mode mode, ViProcessor *processor);
		bool detach(ViProcessor *processor);
		ViAudioBuffer* buffer(ViAudio::Mode mode);

	protected:

		ViAudioBuffer* allocateBuffer(ViAudio::Mode mode);
		void deallocateBuffer(ViAudio::Mode mode);
		ViAudioBuffer* nextBuffer(ViAudio::Mode mode);

		bool isSongRunning();
		bool wasSongRunning();

	private:

		QMutex mMutex;

		ViMultiExecutor mMultiExecutor;
		ViAudioInput *mInput;
		ViStreamOutput *mStreamOutput;
		ViFileOutput *mFileOutput;

		QQueue<ViAudioBuffer*> mInputBuffers;
		QQueue<ViAudioBuffer*> mOutputBuffers;
		ViAudioBuffer *mInputBuffer;
		ViAudioBuffer *mOutputBuffer;

		QList<ViHandler*> mHandlers;
		ViUnderrunHandler *mUnderrunHandler;
		ViProjectHandler *mProjectHandler;
		ViSectionHandler *mSectionHandler;
		ViPlaybackHandler *mPlaybackHandler;
};

#endif
