#ifndef VIWAVEFORMER_H
#define VIWAVEFORMER_H

#include <viwaveform.h>
#include <viprocessor.h>

class ViWaveFormer : public ViProcessor
{

	public:

		ViWaveFormer();
		~ViWaveFormer();

		void clear();

		void initialize();
		void execute(const int &channel);
		void finalize();

		ViWaveForm* wave(); // Ownership stays with ViWaveFormer
		ViWaveForm* takeWave(); // Ownership stransfered to requestor

	private:
		
		ViWaveForm *mForm;

};

#endif
