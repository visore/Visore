#ifndef VISPECTRUMANALYZER_H
#define VISPECTRUMANALYZER_H

#include "vifourierwrapper.h"
#include "viprocessor.h"
#include "vierror.h"
#include "vispectrum.h"
#include "vispectrumwindow.h"
#include "viwaveformchunk.h"
#include <QThread>
#include <QMutex>

// Compile-time calculation of powers of two

template<int N> class PowerOfTwo
{
	public:
		static const int Result = PowerOfTwo<N-1>::Result * 2;
};

template<> class PowerOfTwo<0>
{
	public:
		static const int Result = 1;
};

const int SPECTRUM_SAMPLES = PowerOfTwo<FFT_POWER_OF_TWO>::Result;

class ViSpectrumAnalyzerThread : public QThread
{
	Q_OBJECT

	public:
		ViSpectrumAnalyzerThread();
		~ViSpectrumAnalyzerThread();
		void run();
		void setWindowFunction(ViWindowFunction *windowFunction);
		void addChunk(QSharedPointer<ViWaveFormChunk> chunk);

	private:
		void calculateWindow(qint64 size);
		bool enoughSamplesAvailable();

	private:
		ViWindowFunction *mWindowFunction;
		ViSpectrumWindow *mWindow;
		ViFourierWrapper mFourierWrapper;
		QList<QSharedPointer<ViWaveFormChunk> > mChunks;
		QList<float*> mSamples;
		qint64 mNumberOfSamples;
		QMutex mMutex;
		qint64 mPreviousStop;
};

class ViSpectrumAnalyzer : public ViProcessor, public ViError
{
	Q_OBJECT

	signals:
		void spectrumChanged(const ViSpectrum &spectrum);

	public slots:
		void start(QSharedPointer<ViWaveFormChunk> chunk);

	public:
		ViSpectrumAnalyzer();
		~ViSpectrumAnalyzer();
		void initialize(ViAudioBuffer *buffer);

		void setWindowFunction(ViWindowFunction *windowFunction);
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
