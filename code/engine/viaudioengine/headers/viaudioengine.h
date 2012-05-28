#ifndef VIAUDIOENGINE_H
#define VIAUDIOENGINE_H

#include "viaudioconnection.h"
#include "visingleton.h"

#include "viprocessingchain.h"
#include "viprocessorexecutor.h"

#include "vispectrumanalyzer.h"
#include "viwaveformer.h"
#include "visongdetector.h"

class ViAudioEngine : public ViSingleton
{
    Q_OBJECT

	public slots:

		void calculateSpectrum(qint32 size, QString windowFunction);

	signals:

		void spectrumChanged(short percentage);
		void spectrumFinished();

	public:

		~ViAudioEngine();
		static ViAudioEngine* instance();

		ViRealSpectrum spectrum();

	protected:

		ViAudioEngine();

	private:

		static ViAudioEngine *mEngine;

		ViProcessingChain mProcessingChain;
		ViProcessorExecutor mProcessorExecutor;

		ViSpectrumAnalyzer mSpectrumAnalyzer;

};

#endif
