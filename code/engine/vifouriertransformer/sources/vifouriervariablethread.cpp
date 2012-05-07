#ifdef VIFOURIERVARIABLETHREAD_H

template <typename T>
ViFourierVariablehread<T>::ViFourierVariablehread()
	: QThread()
{
	mInput = NULL;
	mOutput = NULL;
	mNumberOfSamples = 0;
}

template <typename T>
void ViFourierVariablehread<T>::setData(T input[], T output[], int numberOfSamples)
{
	mInput = input;
	mOutput = output;
	mNumberOfSamples = numberOfSamples;
}

template <typename T>
ViFourierVariableForwardThread<T>::ViFourierVariableForwardThread()
	: ViFourierVariablehread<T>()
{
}

template <typename T>
void ViFourierVariableForwardThread<T>::run()
{
	ffft::FFTReal<T> fourierTransform(ViFourierVariablehread<T>::mNumberOfSamples);
	fourierTransform.do_fft(ViFourierVariablehread<T>::mInput, ViFourierVariablehread<T>::mOutput);
}

template <typename T>
ViFourierVariableInverseThread<T>::ViFourierVariableInverseThread()
	: ViFourierVariablehread<T>()
{
}

template <typename T>
void ViFourierVariableInverseThread<T>::run()
{
	ffft::FFTReal<T> fourierTransform(ViFourierVariablehread<T>::mNumberOfSamples);
	fourierTransform.do_ifft(ViFourierVariablehread<T>::mOutput, ViFourierVariablehread<T>::mInput);
}

#endif
