#ifdef VIFOURIERFIXEDTHREAD_H

template <int T>
ViFourierFixedThread<T>::ViFourierFixedThread(ViFourierTransformer *transformer)
	: ViFourierThread(transformer)
{
	setSize(qPow(2, T));
}

template <int T>
ViFourierFixedForwardThread<T>::ViFourierFixedForwardThread(ViFourierTransformer *transformer)
	: ViFourierFixedThread<T>(transformer)
{
}

template <int T>
void ViFourierFixedForwardThread<T>::run()
{
	ViFourierFixedThread<T>::mFourierTransform.do_fft(ViFourierThread::mOutput, ViFourierThread::mInput);
	ViFourierThread::notify();
}

template <int T>
ViFourierFixedInverseThread<T>::ViFourierFixedInverseThread(ViFourierTransformer *transformer)
	: ViFourierFixedThread<T>(transformer)
{
}

template <int T>
void ViFourierFixedInverseThread<T>::run()
{
	ViFourierFixedThread<T>::mFourierTransform.do_ifft(ViFourierThread::mInput, ViFourierThread::mOutput);
	ViFourierThread::notify();
}

template <int T>
ViFourierFixedRescaleThread<T>::ViFourierFixedRescaleThread(ViFourierTransformer *transformer)
	: ViFourierFixedThread<T>(transformer)
{
}

template <int T>
void ViFourierFixedRescaleThread<T>::run()
{
	ViFourierFixedThread<T>::mFourierTransform.rescale(ViFourierThread::mInput);
	ViFourierThread::notify();
}

#endif
