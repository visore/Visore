#include <vifrequencynoisedetector.h>

#define FREQUENCY_THRESHOLD 0.002
#define FREQUENCY_FROM 0.9 // Start percentage of the spectrum to observe
#define FREQUENCY_TO 1.0 // End percentage of the spectrum to observe

bool ViFrequencyNoiseDetector::calculateNoise()
{
	ViFrequencyChunk &chunk = frequencies();
    int end = chunk.size() * FREQUENCY_TO;
    qreal total = 0;
    for(int i = chunk.size() * FREQUENCY_FROM; i < end; ++i)
    {
        total += chunk[i];
    }
	return (total / chunk.size()) > FREQUENCY_THRESHOLD;
}

ViFrequencyNoiseDetector* ViFrequencyNoiseDetector::clone()
{
    return new ViFrequencyNoiseDetector(*this);
}

#ifdef __cplusplus
extern "C"
{
#endif

ViFrequencyNoiseDetector* create()
{
    return new ViFrequencyNoiseDetector();
}

#ifdef __cplusplus
}
#endif
