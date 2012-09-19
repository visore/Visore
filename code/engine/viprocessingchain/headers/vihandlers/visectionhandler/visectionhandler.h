#ifndef VISECTIONHANDLER_H
#define VISECTIONHANDLER_H

#include "vihandler.h"
#include "vienddetector.h"
#include "vispectrumanalyzer.h"
#include "viaudioobject.h"
#include <QMutexLocker>

class ViSectionHandler : public ViHandler
{

    Q_OBJECT

	private slots:

		void startRecord();
		void endRecord();
		void startSong();
		void endSong();

		void startInput(bool isSong = false);
		void endInput();
		void finish();
		void startOutput();
		void endOutput();

		void setDetector(ViProcessor *processor);

	public:

		ViSectionHandler(ViProcessingChain *chain);
		~ViSectionHandler();

		bool isSongRunning();
		bool wasSongRunning();

	private:

		ViAudioInput* input();
		ViAudioOutput* output();
		ViExecutor* executor();

		ViAudioBuffer* allocateBuffer();
		void deallocateBuffer(ViAudioBuffer *buffer);

	private:

		QMutex mMutex;

		ViEndDetector *mEndDetector;
		ViSpectrumAnalyzer *mSpectrumAnalyzer;

		bool mIsSongRunning;
		bool mWasSongRunning;
		bool mAcceptFinish;
		bool mPlayAutomatically;
		bool mIsPlaying;

		QQueue<ViAudioObject*> mNoSongObjects;

};

#endif
