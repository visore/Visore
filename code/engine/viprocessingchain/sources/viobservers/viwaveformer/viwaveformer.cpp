#include "viwaveformer.h"

ViWaveFormer::ViWaveFormer()
	: ViObserver()
{
}

ViWaveForm& ViWaveFormer::wave()
{
	return mForm;
}

void ViWaveFormer::initialize()
{
	mForm.clear();
}

void ViWaveFormer::execute()
{
	for(int i = 0; i < mData->size(); ++i)
	{
		mForm.append(mData->at(i));
	}
}
