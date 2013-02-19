#ifndef VIWAVEFORMER_H
#define VIWAVEFORMER_H

#include "viwaveform.h"
#include "viprocessor.h"

class ViWaveFormer : public ViProcessor
{

	public:

		ViWaveFormer();

		void initialize();
		void execute();
		void finalize();

	private:
		
		ViWaveForm *mForm;

};

#endif
