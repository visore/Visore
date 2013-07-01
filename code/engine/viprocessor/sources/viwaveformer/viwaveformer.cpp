#include "viwaveformer.h"

ViWaveFormer::ViWaveFormer()
	: ViProcessor()
{
	mForm = NULL;
}

void ViWaveFormer::initialize()
{
	mForm = new ViWaveForm();
}

void ViWaveFormer::execute(int channel)
{
/*	ViSampleChunk &theSamples = samples();
	for(int i = 0; i < theSamples.size(); ++i)
	{
		mForm->append(theSamples[i]);
    }*/
}

void ViWaveFormer::finalize()
{
	object()->setWaveForm(type(), mForm);
}
