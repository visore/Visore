#include "vifouriervariablethread.h"

ViFourierVariableThread::ViFourierVariableThread()
	: ViFourierThread()
{
}

ViFourierVariableForwardThread::ViFourierVariableForwardThread()
	: ViFourierVariableThread()
{
}

void ViFourierVariableForwardThread::run()
{
	ffft::FFTReal<float> fourierTransform(mSize);
	fourierTransform.do_fft(mOutput, mInput);
}

ViFourierVariableInverseThread::ViFourierVariableInverseThread()
	: ViFourierVariableThread()
{
}

void ViFourierVariableInverseThread::run()
{
	ffft::FFTReal<float> fourierTransform(mSize);
	fourierTransform.do_ifft(mInput, mOutput);
}
