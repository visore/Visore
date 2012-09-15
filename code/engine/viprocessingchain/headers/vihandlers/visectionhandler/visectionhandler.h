#ifndef VISECTIONHANDLER_H
#define VISECTIONHANDLER_H

#include "vihandler.h"
#include "vienddetector.h"
#include "vispectrumanalyzer.h"

class ViSectionHandler : public ViHandler
{

    Q_OBJECT

	private slots:

		void startRecord();
		void endRecord();
		void startSong();
		void endSong();

		void startInput();
		void endInput();
		void finish();
		void startOutput();
		void endOutput();

		void setDetector(ViProcessor *processor);

	public:

		ViSectionHandler(ViProcessingChain *chain);
		bool isSongRunning();
		bool wasSongRunning();

	private:

		ViAudioInput* input();
		ViAudioOutput* output();
		ViExecutor* executor();

		ViAudioBuffer* allocateBuffer(ViAudio::Mode mode);
		void deallocateBuffer(ViAudioBuffer *buffer);
		ViAudioBuffer* nextBuffer(ViAudio::Mode mode);

	private:

		ViEndDetector *mEndDetector;
		ViSpectrumAnalyzer *mSpectrumAnalyzer;

		bool mIsSongRunning;
		bool mWasSongRunning;
		bool mAcceptFinish;
		bool mPlayAutomatically;
		bool mIsPlaying;

		QQueue<ViAudioBuffer*> mInputBuffers;
		QQueue<ViAudioBuffer*> mOutputBuffers;
		ViAudioBuffer *mInputBuffer;
		ViAudioBuffer *mOutputBuffer;

};

#endif
