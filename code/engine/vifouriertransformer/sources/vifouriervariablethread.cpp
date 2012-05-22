#include "vifouriervariablethread.h"

ViFourierVariableThread::ViFourierVariableThread(ViFourierTransformer *transformer)
	: ViFourierThread(transformer)
{
}

ViFourierVariableForwardThread::ViFourierVariableForwardThread(ViFourierTransformer *transformer)
	: ViFourierVariableThread(transformer)
{
}

void ViFourierVariableForwardThread::run()
{
	ffft::FFTReal<float> fourierTransform(mSize);
	fourierTransform.do_fft(mOutput, mInput);
	ViFourierThread::notify();
}

ViFourierVariableInverseThread::ViFourierVariableInverseThread(ViFourierTransformer *transformer)
	: ViFourierVariableThread(transformer)
{
}

void ViFourierVariableInverseThread::run()
{
	ffft::FFTReal<float> fourierTransform(mSize);
	fourierTransform.do_ifft(mInput, mOutput);
	ViFourierThread::notify();
}

ViFourierVariableRescaleThread::ViFourierVariableRescaleThread(ViFourierTransformer *transformer)
	: ViFourierVariableThread(transformer)
{
}

void ViFourierVariableRescaleThread::run()
{
	ffft::FFTReal<float> fourierTransform(mSize);
	fourierTransform.rescale(mInput);
	ViFourierThread::notify();
}
