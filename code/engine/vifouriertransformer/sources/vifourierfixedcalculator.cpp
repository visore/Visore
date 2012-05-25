#ifdef VIFOURIERFIXEDCALCULATOR_H

template <int T>
ViFourierFixedCalculator<T>::ViFourierFixedCalculator()
	: ViFourierCalculator()
{
	setSize(mFourierTransform.get_length());
}

template <int T>
void ViFourierFixedCalculator<T>::setSize(int size)
{
}

template <int T>
void ViFourierFixedCalculator<T>::forward()
{
	ViFourierFixedCalculator<T>::mFourierTransform.do_fft(ViFourierCalculator::mOutput, ViFourierCalculator::mInput);
}

template <int T>
void ViFourierFixedCalculator<T>::inverse()
{
	ViFourierFixedCalculator<T>::mFourierTransform.do_ifft(ViFourierCalculator::mInput, ViFourierCalculator::mOutput);
}

template <int T>
void ViFourierFixedCalculator<T>::rescale()
{
	ViFourierFixedCalculator<T>::mFourierTransform.rescale(ViFourierCalculator::mInput);
}

#endif
