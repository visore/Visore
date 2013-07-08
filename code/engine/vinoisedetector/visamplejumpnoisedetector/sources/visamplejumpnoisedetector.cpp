#include <visamplejumpnoisedetector.h>

#define JUMP_THRESHOLD 0.5 // The difference between samples that is considered as a jump

bool ViSampleJumpNoiseDetector::isNoisy()
{
	ViSampleChunk &chunk = samples();
    int end = chunk.size() - 1;
    for(int i = 0; i < end; ++i)
    {
        if(qAbs(chunk[i] - chunk[i + 1]) > JUMP_THRESHOLD)
        {
            return true;
        }
    }
	return false;
}

ViSampleJumpNoiseDetector* ViSampleJumpNoiseDetector::clone()
{
    return new ViSampleJumpNoiseDetector(*this);
}

#ifdef __cplusplus
extern "C"
{
#endif

ViSampleJumpNoiseDetector* create()
{
    return new ViSampleJumpNoiseDetector();
}

#ifdef __cplusplus
}
#endif
