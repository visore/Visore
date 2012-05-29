#include "viwaveformer.h"

ViWaveFormer::ViWaveFormer()
	: ViObserver()
{
}

ViWaveForm& ViWaveFormer::wave()
{
	return mForm;
}

void ViWaveFormer::run()
{
	for(int i = 0; i < mData->size(); ++i)
	{
		mForm.append(mData->at(i));
	}
}
