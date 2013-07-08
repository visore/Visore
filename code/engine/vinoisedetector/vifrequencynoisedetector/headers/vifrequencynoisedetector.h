#ifndef VIFREQUENCYNOISEDETECTOR_H
#define VIFREQUENCYNOISEDETECTOR_H

#include <vinoisedetector.h>

class ViFrequencyNoiseDetector : public ViNoiseDetector
{

    public:

        ViFrequencyNoiseDetector();
        ViFrequencyNoiseDetector(const ViFrequencyNoiseDetector &other);
        ~ViFrequencyNoiseDetector();

        bool isNoisy();

        ViFrequencyNoiseDetector* clone();

};

#endif

