#ifndef VISAMPLEJUMPNOISEDETECTOR_H
#define VISAMPLEJUMPNOISEDETECTOR_H

#include <vinoisedetector.h>

class ViSampleJumpNoiseDetector : public ViNoiseDetector
{

    public:

        ViSampleJumpNoiseDetector* clone();

	protected:

		bool calculateNoise();

};

#endif

