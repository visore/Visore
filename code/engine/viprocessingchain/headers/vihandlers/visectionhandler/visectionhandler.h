#ifndef VISECTIONHANDLER_H
#define VISECTIONHANDLER_H

#include "vihandler.h"
#include "vienddetector.h"
#include "vispectrumanalyzer.h"
#include "viaudioobject.h"
#include <QMutexLocker>
#include <QTimer>

class ViSectionHandler : public ViHandler
{

    Q_OBJECT

	signals:

		void songStarted();
		void songEnded();
		void recordStarted();
		void recordEnded();

	private slots:

		void startRecord();
		void endRecord();
		void startSong();
		void endSong();

		void startInput(bool isSong = false);
		void endInput();

		void setDetector(ViProcessor *processor);

		void checkSize();

	public:

		ViSectionHandler(ViProcessingChain *chain);
		~ViSectionHandler();

		ViAudioObjectPointer currentObject();
		bool isSongRunning();
		bool wasSongRunning();
		void setIdleSize(qint64 bytes);

	private:

		ViAudioInput* input();
		ViAudioOutput* output();
		ViExecutor* executor();

		ViBuffer* allocateBuffer();
		void deallocateBuffer(ViBuffer *buffer);

	private:

		ViEndDetector *mEndDetector;
		ViSpectrumAnalyzer *mSpectrumAnalyzer;

		qint64 mIdleSize;
		QTimer mIdleTimer;

		bool mIsSongRunning;
		bool mWasSongRunning;
		bool mAcceptFinish;
		bool mPlayAutomatically;
		bool mIsPlaying;

		ViAudioObjectPointer mCurrentSongObject;
		ViAudioObjectQueue mNoSongObjects;

};

#endif
