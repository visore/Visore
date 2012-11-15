#ifndef VIPROCESSINGCHAIN_H
#define VIPROCESSINGCHAIN_H

#include <QQueue>
#include <QTime>
#include <QTimer>
#include "viaudioconnection.h"
#include "vimultiexecutor.h"
#include "viaudioobjectqueue.h"
#include "viproject.h"

class ViHandler;
class ViUnderrunHandler;
class ViSectionHandler;
class ViPlaybackHandler;

class ViProcessingChain : public QObject
{
    Q_OBJECT

	friend class ViHandler;
	friend class ViUnderrunHandler;
	friend class ViSectionHandler;
	friend class ViPlaybackHandler;

	signals:

		void changed();
		void progressStarted();
		void progress(short progress);
		void progressFinished();
		void statusChanged(QString status);

		void inputChanged();
		void streamOutputChanged(ViStreamOutput *output);
		void fileOutputChanged(ViFileOutput *output);
		void attached(ViProcessor *processor);

		void songStarted();
		void songEnded();
		void recordStarted();
		void recordEnded();

	private slots:

		void updateProject(ViAudioObjectPointer object);

	public:

		ViProcessingChain();
		~ViProcessingChain();

		void setWindowSize(int windowSize);
		void setTransmission(ViAudioTransmission *transmission);
		void setProject(ViProject *project);
		bool attach(ViAudio::Mode mode, ViProcessor *processor);
		bool detach(ViProcessor *processor);

		ViAudioObjectPointer recordingObject();
		ViAudioObjectPointer playingObject();

	protected:

		bool isSongRunning();
		bool wasSongRunning();

	private:

		QMutex mMutex;

		ViMultiExecutor mMultiExecutor;
		ViAudioInput *mInput;
		ViStreamOutput *mStreamOutput;
		ViFileOutput *mFileOutput;

		QList<ViHandler*> mHandlers;
		ViUnderrunHandler *mUnderrunHandler;
		ViSectionHandler *mSectionHandler;
		ViPlaybackHandler *mPlaybackHandler;

		ViAudioObjectQueue mAudioObjects;

		ViProject *mProject;
};

#endif
