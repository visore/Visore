#ifndef VIWAVEFORMER_H
#define VIWAVEFORMER_H

#include "viwaveform.h"
#include "viprocessor.h"

class ViWaveFormer : public ViObserver
{
	Q_OBJECT

	signals:

		void changed(ViWaveForm *waveForm);

	public:

		ViWaveFormer();
		ViWaveForm& waveSummary();
		void run();

	private:
		
		ViWaveForm mForm;
		int mSignalCounter;

};

#endif
