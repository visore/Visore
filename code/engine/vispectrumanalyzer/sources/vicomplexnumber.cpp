#ifdef VICOMPLEXNUMBER_H

template <typename T>
ViComplexNumber<T>::ViComplexNumber()
{
	mReal = 0;
	mImaginary = 0;
}

template <typename T>
ViComplexNumber<T>::ViComplexNumber(T real, T imaginary)
{
	mReal = real;
	mImaginary = imaginary;
}

template <typename T>
void ViComplexNumber<T>::setReal(T real)
{
	mReal = real;
}

template <typename T>
void ViComplexNumber<T>::setImaginary(T imaginary)
{
	mImaginary = imaginary;
}

template <typename T>
void ViComplexNumber<T>::set(T real, T imaginary)
{
	mReal = real;
	mImaginary = imaginary;
}

template <typename T>
void ViComplexNumber<T>::set(ViComplexNumber complex)
{
	mReal = complex.real();
	mImaginary = complex.imaginary();
}

template <typename T>
T ViComplexNumber<T>::real()
{
	return mReal;
}

template <typename T>
T ViComplexNumber<T>::imaginary()
{
	return mImaginary;
}

#endif
