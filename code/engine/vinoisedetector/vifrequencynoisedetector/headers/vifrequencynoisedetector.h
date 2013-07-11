#ifndef VIFREQUENCYNOISEDETECTOR_H
#define VIFREQUENCYNOISEDETECTOR_H

#include <vinoisedetector.h>

class ViFrequencyNoiseDetector : public ViNoiseDetector
{

    public:

        ViFrequencyNoiseDetector* clone();

	protected:

		bool calculateNoise();

};

#endif

