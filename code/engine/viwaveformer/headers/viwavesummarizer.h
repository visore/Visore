#ifndef VIWAVESUMMARIZER_H
#define VIWAVESUMMARIZER_H

#include <QMutex>
#include "viwaveform.h"
#include "viwaveformer.h"
#include "viprocessor.h"

class ViWaveSummarizer;

class ViWaveSummarizerThread : public ViProcessorThread
{
	Q_OBJECT

	public:
		ViWaveSummarizerThread(ViAudioBuffer *buffer, ViWaveForm *form, QIODevice::OpenMode mode);
		void run();

	private:
		ViWaveFormer mFormer;
		ViWaveForm *mForm;
		int mSignalCounter;
};

class ViWaveSummarizer : public ViProcessor
{
	Q_OBJECT

	public:
		ViWaveSummarizer();
		void initialize(ViAudioBuffer *buffer);
		ViWaveForm* waveSummary();

	private:
		ViWaveForm mForm;
};

#endif
