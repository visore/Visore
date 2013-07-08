#include <visamplecorrelator.h>

ViSampleCorrelator::ViSampleCorrelator()
	: ViCorrelator()
{
}

ViSampleCorrelator::ViSampleCorrelator(const ViSampleCorrelator &other)
    : ViCorrelator(other)
{
}

void ViSampleCorrelator::execute(int channel)
{
	ViSampleChunk &theSamples1 = currentSamples();
	ViSampleChunk &theSamples2 = currentSamples2();

	qint32 size = qMin(theSamples1.size(), theSamples2.size());
	for(int i = 0; i < size; ++i)
	{
        addCorrelation(1 - (qAbs(theSamples1[i] - theSamples2[i]) / 2));
	}
}

ViSampleCorrelator* ViSampleCorrelator::clone()
{
    return new ViSampleCorrelator(*this);
}

#ifdef __cplusplus
extern "C"
{
#endif

ViSampleCorrelator* create()
{
    return new ViSampleCorrelator();
}

#ifdef __cplusplus
}
#endif
