#include "vifourierfixedthread.h"

ViFourierFixedThread::ViFourierFixedThread()
	: QThread()
{
	mInput = NULL;
	mOutput = NULL;
}

void ViFourierFixedThread::setData(float input[], float output[])
{
	mInput = input;
	mOutput = output;
}

ViFourierFixedForwardThread::ViFourierFixedForwardThread()
	: ViFourierFixedThread()
{
}

void ViFourierFixedForwardThread::run()
{
	mFourierTransform.do_fft(mOutput, mInput);
}

ViFourierFixedInverseThread::ViFourierFixedInverseThread()
	: ViFourierFixedThread()
{
}

void ViFourierFixedInverseThread::run()
{
	mFourierTransform.do_ifft(mInput, mOutput);
}
