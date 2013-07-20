#include <viwaveformer.h>

ViWaveFormer::ViWaveFormer()
	: ViProcessor()
{
	mForm = NULL;
}

ViWaveFormer::~ViWaveFormer()
{
	clear();
}

void ViWaveFormer::clear()
{
	if(mForm != NULL)
	{
		delete mForm;
		mForm = NULL;
	}
	ViProcessor::clear();
}

void ViWaveFormer::initialize()
{
	setChannelMode(ViProcessor::Combined);
	setProcessMode(ViProcessor::All);
	if(mForm != NULL)
	{
		delete mForm;
	}
	mForm = new ViWaveForm();
}

void ViWaveFormer::execute(const int &channel)
{
	ViSampleChunk &theSamples = currentSamples();
	for(int i = 0; i < theSamples.size(); ++i)
	{
		mForm->append(theSamples[i]);
	}
}

void ViWaveFormer::finalize()
{
}

ViWaveForm* ViWaveFormer::wave()
{
	return mForm;
}

ViWaveForm* ViWaveFormer::takeWave()
{
	ViWaveForm *result = mForm;
	mForm = NULL;
	return result;
}
