#ifdef VIFOURIERFIXEDTHREAD_H

template <int T>
ViFourierFixedThread<T>::ViFourierFixedThread()
	: ViFourierThread()
{
	setSize(qPow(2, T));
}

template <int T>
ViFourierFixedForwardThread<T>::ViFourierFixedForwardThread()
	: ViFourierFixedThread<T>()
{
}

template <int T>
void ViFourierFixedForwardThread<T>::run()
{
	ViFourierFixedThread<T>::mFourierTransform.do_fft(ViFourierThread::mOutput, ViFourierThread::mInput);
}

template <int T>
ViFourierFixedInverseThread<T>::ViFourierFixedInverseThread()
	: ViFourierFixedThread<T>()
{
}

template <int T>
void ViFourierFixedInverseThread<T>::run()
{
	ViFourierFixedThread<T>::mFourierTransform.do_ifft(ViFourierThread::mInput, ViFourierThread::mOutput);
}

#endif
