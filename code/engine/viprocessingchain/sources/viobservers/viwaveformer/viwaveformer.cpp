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
	for(int i = 0; i < mInputData->size(); ++i)
	{
		mForm.append(mInputData->at(i));
	}
}
