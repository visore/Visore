#include "vifourierfixedcalculator.h"

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

template class ViFourierFixedCalculator<3>;
template class ViFourierFixedCalculator<4>;
template class ViFourierFixedCalculator<5>;
template class ViFourierFixedCalculator<6>;
template class ViFourierFixedCalculator<7>;
template class ViFourierFixedCalculator<8>;
template class ViFourierFixedCalculator<9>;
template class ViFourierFixedCalculator<10>;
template class ViFourierFixedCalculator<11>;
template class ViFourierFixedCalculator<12>;
template class ViFourierFixedCalculator<13>;
template class ViFourierFixedCalculator<14>;
