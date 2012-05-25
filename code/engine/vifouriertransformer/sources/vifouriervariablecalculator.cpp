#include "vifouriervariablecalculator.h"

ViFourierVariableCalculator::ViFourierVariableCalculator()
	: ViFourierCalculator()
{
	mFourierTransform = 0;
}

ViFourierVariableCalculator::~ViFourierVariableCalculator()
{
	if(mFourierTransform != 0)
	{
		delete mFourierTransform;
	}
}

void ViFourierVariableCalculator::setSize(int size)
{
	ViFourierCalculator::setSize(size);
	if(mFourierTransform == 0)
	{
		mFourierTransform = new ffft::FFTReal<float>(mSize);
	}
	else if(mFourierTransform->get_length() != mSize)
	{
		delete mFourierTransform;
		mFourierTransform = new ffft::FFTReal<float>(mSize);
	}
}

void ViFourierVariableCalculator::forward()
{
	mFourierTransform->do_fft(mOutput, mInput);
}

void ViFourierVariableCalculator::inverse()
{
	mFourierTransform->do_ifft(mInput, mOutput);
}

void ViFourierVariableCalculator::rescale()
{
	mFourierTransform->rescale(mInput);
}
