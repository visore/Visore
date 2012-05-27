#ifndef VISPECTRUMANALYZER_H
#define VISPECTRUMANALYZER_H

#include "vispectrum.h"
#include "viprocessor.h"
#include "vifouriertransformer.h"

class ViSpectrumAnalyzer : public ViObserver
{

	public:

		ViSpectrumAnalyzer();
		ViFloatSpectrum& spectrum();
		void setWindowFunction(QString functionName);

		void setWindowSize(int windowSize);
		void setFormat(ViAudioFormat format);
		void run();

	private:

		ViFloatSpectrum mSpectrum;
		QString mWindowFunction;

};

#endif
