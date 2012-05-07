#ifdef VIFOURIERFIXEDTHREAD_H

template <typename T>
ViFourierFixedThread<T>::ViFourierFixedThread()
	: QThread()
{
	mInput = NULL;
	mOutput = NULL;
}

template <typename T>
void ViFourierFixedThread<T>::setData(T input[], T output[])
{
	mInput = input;
	mOutput = output;
}

template <typename T>
ViFourierFixedForwardThread<T>::ViFourierFixedForwardThread()
	: ViFourierFixedThread<T>()
{
}

template <typename T>
void ViFourierFixedForwardThread<T>::run()
{
	ViFourierFixedThread<T>::mFourierTransform.mFourierTransform.do_fft(ViFourierFixedThread<T>::mOutput, ViFourierFixedThread<T>::mInput);
}

template <typename T>
ViFourierFixedInverseThread<T>::ViFourierFixedInverseThread()
	: ViFourierFixedThread<T>()
{
}

template <typename T>
void ViFourierFixedInverseThread<T>::run()
{
	ViFourierFixedThread<T>::mFourierTransform.mFourierTransform.do_ifft(ViFourierFixedThread<T>::mOutput, ViFourierFixedThread<T>::mInput);
}

#endif
