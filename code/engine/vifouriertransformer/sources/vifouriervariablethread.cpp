#include "vifouriervariablethread.h"

ViFourierVariablehread::ViFourierVariablehread()
	: QThread()
{
	mInput = NULL;
	mOutput = NULL;
	mNumberOfSamples = 0;
}

void ViFourierVariablehread::setData(float input[], float output[], int numberOfSamples)
{
	mInput = input;
	mOutput = output;
	mNumberOfSamples = numberOfSamples;
}

ViFourierVariableForwardThread::ViFourierVariableForwardThread()
	: ViFourierVariablehread()
{
}

void ViFourierVariableForwardThread::run()
{
	ffft::FFTReal<float> fourierTransform(mNumberOfSamples);
	fourierTransform.do_fft(mOutput, mInput);
}

ViFourierVariableInverseThread::ViFourierVariableInverseThread()
	: ViFourierVariablehread()
{
}

void ViFourierVariableInverseThread::run()
{
	ffft::FFTReal<float> fourierTransform(mNumberOfSamples);
	fourierTransform.do_ifft(mInput, mOutput);
}
