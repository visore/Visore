#ifndef VISAMPLECORRELATOR_H
#define VISAMPLECORRELATOR_H

#include <vicorrelator.h>

class ViSampleCorrelator : public ViCorrelator
{

	public:
		
		ViSampleCorrelator();
        ViSampleCorrelator(const ViSampleCorrelator &other);

		void execute(const int &channel);

        ViSampleCorrelator* clone();
		
};

#endif
