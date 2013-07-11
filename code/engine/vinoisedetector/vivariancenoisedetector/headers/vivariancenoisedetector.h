#ifndef VIVARIANCENOISEDETECTOR_H
#define VIVARIANCENOISEDETECTOR_H

#include <vinoisedetector.h>

class ViVarianceNoiseDetector : public ViNoiseDetector
{

    public:

        ViVarianceNoiseDetector* clone();

	protected:

		bool calculateNoise();

};

#endif
