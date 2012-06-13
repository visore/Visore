#ifndef VIWAVEFORMER_H
#define VIWAVEFORMER_H

#include "viwaveform.h"
#include "viprocessor.h"

class ViWaveFormer : public ViObserver
{

	public:

		ViWaveFormer();
		ViWaveForm& wave();
		void initialize();
		void run();

	private:
		
		ViWaveForm mForm;

};

#endif
