#ifndef VISPECTRUMANALYZER_H
#define VISPECTRUMANALYZER_H

#include "vifourierwrapper.h"
#include "viprocessor.h"
#include "vierror.h"

class ViSpectrumAnalyzer : public ViProcessor, public ViError
{
    Q_OBJECT

	signals:
		void spectrumChanged(const ViFrequencySpectrum &spectrum);

	private slots:
		void calculationComplete(const ViFrequencySpectrum &spectrum);

	public:
		ViSpectrumAnalyzer(QObject *parent = 0);
		~ViSpectrumAnalyzer();

		void setWindowFunction(ViWindowFunction type);
		void start(QByteArray &buffer, QAudioFormat &format);
		void stop();
		bool isReady();

	private:
		enum ViProcessState
		{
			Idle,
			Busy,
			Cancelled
		};

		void calculateWindow();

	private:
		ViSpectrumAnalyzerThread* mThread;
		ViProcessState mState;
};

#endif
