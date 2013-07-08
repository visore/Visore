
#include <vivariancenoisedetector.h>

#define THRESHOLD 0.3

ViVarianceNoiseDetector::ViVarianceNoiseDetector()
    : ViNoiseDetector()
{
}

ViVarianceNoiseDetector::ViVarianceNoiseDetector(const ViVarianceNoiseDetector &other)
    : ViNoiseDetector(other)
{
}

ViVarianceNoiseDetector::~ViVarianceNoiseDetector()
{
}

bool ViVarianceNoiseDetector::isNoisy()
{
    /*ViSampleChunk &chunk = processor()->samples();
    qreal total = 0;
    int size = chunk.size() - 1;
    for(int i = 0; i < size; ++i)
    {
        total += qAbs(chunk[i] - chunk[i + 1]);
    }
    return (total / size) > THRESHOLD;*/
}

ViVarianceNoiseDetector* ViVarianceNoiseDetector::clone()
{
    return new ViVarianceNoiseDetector(*this);
}

#ifdef __cplusplus
extern "C"
{
#endif

ViVarianceNoiseDetector* create()
{
    return new ViVarianceNoiseDetector();
}

#ifdef __cplusplus
}
#endif
