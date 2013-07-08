#ifndef VIVARIANCENOISEDETECTOR_H
#define VIVARIANCENOISEDETECTOR_H

#include <vinoisedetector.h>

class ViVarianceNoiseDetector : public ViNoiseDetector
{

    public:

        ViVarianceNoiseDetector();
        ViVarianceNoiseDetector(const ViVarianceNoiseDetector &other);
        ~ViVarianceNoiseDetector();

        bool isNoisy();

        ViVarianceNoiseDetector* clone();

};

#endif
