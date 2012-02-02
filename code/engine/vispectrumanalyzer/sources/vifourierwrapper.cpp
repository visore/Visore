#include "vifourierwrapper.h"

ViFourierWrapper::ViFourierWrapper()
{
	mPrivate = new ViFourierWrapperPrivate();
}

ViFourierWrapper::~ViFourierWrapper()
{
	delete mPrivate;
}

void ViFourierWrapper::calculateFft(DataType in[], const DataType out[])
{
	mPrivate->mFourierTransform.do_fft(in, out);
}
