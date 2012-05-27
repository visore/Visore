#ifndef VISPECTRUMANALYZER_H
#define VISPECTRUMANALYZER_H

#include <QMutex>
#include "vispectrum.h"
#include "viprocessor.h"
#include "vifouriertransformer.h"

class ViSpectrumAnalyzer : public ViObserver
{

	public:

		ViSpectrumAnalyzer();
		ViRealSpectrum spectrum();
		void setWindowFunction(QString functionName);

		void setWindowSize(int windowSize);
		void setFormat(ViAudioFormat format);
		void run();

	private:

		QMutex mMutex;
		int mHalfWindowSize;
		ViFourierTransformer mTransformer;
		ViRealSpectrum mSpectrum;

};

#endif
