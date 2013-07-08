#include "viwaveformer.h"

ViWaveFormer::ViWaveFormer()
	: ViProcessor()
{
	mForm = NULL;
}

void ViWaveFormer::initialize()
{
	setChannelMode(ViProcessor::Combined);
	mForm = new ViWaveForm();
}

void ViWaveFormer::execute(int channel)
{
	ViSampleChunk &theSamples = currentSamples();
	for(int i = 0; i < theSamples.size(); ++i)
	{
		mForm->append(theSamples[i]);
	}
}

void ViWaveFormer::finalize()
{
	object()->setWaveForm(type(), mForm);
}
