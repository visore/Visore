#ifndef VISPECTRUMANALYZER_H
#define VISPECTRUMANALYZER_H

#include "vifourierwrapper.h"
#include "viprocessor.h"
#include "vierror.h"
#include "vispectrum.h"
#include "vispectrumwindow.h"
#include "viwaveformchunk.h"
#include <QThread>

class ViSpectrumAnalyzerThread : public QThread
{
	Q_OBJECT

	public:
		void start();
		void setWindowFunction(ViWindowFunction *windowFunction);
		void addChunk(ViWaveFormChunk *chunk);

	private:
		void addWindow(qint64 size);

	private:
		ViWindowFunction *mWindowFunction;
		QList<ViSpectrumWindow*> mWindows;
		QList<ViWaveFormChunk*> mChunks;
		qint64 mNumberOfSamples;
};

class ViSpectrumAnalyzer : public ViProcessor, public ViError
{
	Q_OBJECT

	signals:
		void spectrumChanged(const ViSpectrum &spectrum);

	public:
		ViSpectrumAnalyzer();
		~ViSpectrumAnalyzer();

		void setWindowFunction(ViWindowFunction *windowFunction);
		void start(ViWaveFormChunk *chunk);
		void stop();
		bool isReady();

	private:
		enum ViProcessState
		{
			Idle,
			Busy,
			Cancelled
		};

	private:
		ViSpectrumAnalyzerThread *mThread;
		ViProcessState mState;
};

#endif
