#include "viwaveformer.h"

#define MAX_SIGNAL_COUNTER 100

ViWaveFormer::ViWaveFormer()
	: ViRealProcessor()
{
	mSignalCounter = 0;
}

ViWaveForm& ViWaveFormer::waveSummary()
{
	return mForm;
}

void ViWaveFormer::run()
{
	for(int i = 0; i < mData->size(); ++i)
	{
		mForm.append(mData->at(i));
	}
	++mSignalCounter;
	if(mSignalCounter > MAX_SIGNAL_COUNTER)
	{
		mSignalCounter = 0;
		emit changed(&mForm);
	}
}
