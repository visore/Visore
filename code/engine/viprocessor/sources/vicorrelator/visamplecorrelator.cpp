#include <visamplecorrelator.h>

ViSampleCorrelator::ViSampleCorrelator()
	: ViCorrelator()
{
}

void ViSampleCorrelator::execute()
{
	ViSampleChunk &theSamples1 = samples1();
	ViSampleChunk &theSamples2 = samples2();

	qint32 size = qMin(theSamples1.size(), theSamples2.size());
	for(int i = 0; i < size; ++i)
	{
        addCorrelation(qAbs(theSamples1[i] - theSamples2[i]));
	}
}
