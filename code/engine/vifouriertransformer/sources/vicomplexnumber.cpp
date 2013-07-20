#include "vicomplexnumber.h"

template <typename T>
ViComplexNumber<T>::ViComplexNumber()
{
	mReal = 0;
	mImaginary = 0;
}

template <typename T>
ViComplexNumber<T>::ViComplexNumber(const ViComplexNumber<T> &other)
{
	mReal = other.mReal;
	mImaginary = other.mImaginary;
}

template <typename T>
ViComplexNumber<T>::ViComplexNumber(const T &real, const T &imaginary)
{
	mReal = real;
	mImaginary = imaginary;
}

template <typename T>
void ViComplexNumber<T>::setReal(const T &real)
{
	mReal = real;
}

template <typename T>
void ViComplexNumber<T>::setImaginary(const T &imaginary)
{
	mImaginary = imaginary;
}

template <typename T>
void ViComplexNumber<T>::set(const T &real, const T &imaginary)
{
	mReal = real;
	mImaginary = imaginary;
}

template <typename T>
void ViComplexNumber<T>::set(const ViComplexNumber &complex)
{
	mReal = complex.real();
	mImaginary = complex.imaginary();
}

template <typename T>
const T& ViComplexNumber<T>::real() const
{
	return mReal;
}

template <typename T>
const T& ViComplexNumber<T>::imaginary() const
{
	return mImaginary;
}

template <typename T>
bool ViComplexNumber<T>::operator == (const ViComplexNumber<T> &complex) const
{
	return (mReal == complex.mReal) && (mImaginary == complex.mImaginary);
}

template <typename T>
bool ViComplexNumber<T>::operator != (const ViComplexNumber<T> &complex) const
{
	return !(*this == complex);
}

template <typename T>
const ViComplexNumber<T>& ViComplexNumber<T>::operator += (const ViComplexNumber<T> &complex)
{
	mReal += complex.mReal;
	mImaginary += complex.mImaginary;
	return *this;
}

template <typename T>
const ViComplexNumber<T>& ViComplexNumber<T>::operator -= (const ViComplexNumber<T> &complex)
{
	mReal -= complex.mReal;
	mImaginary -= complex.mImaginary;
	return *this;
}

template <typename T>
const ViComplexNumber<T>& ViComplexNumber<T>::operator /= (const T &value)
{
	mReal /= value;
	mImaginary /= value;
	return *this;
}

template <typename T>
const ViComplexNumber<T>& ViComplexNumber<T>::operator *= (const T &value)
{
	mReal *= value;
	mImaginary *= value;
	return *this;
}

template <typename T>
ViComplexNumber<T> ViComplexNumber<T>::operator / (const T &value) const
{
	ViComplexNumber<T> result(mReal / value, mImaginary / value);
	return result;
}

template <typename T>
ViComplexNumber<T> ViComplexNumber<T>::operator * (const T &value) const
{
	ViComplexNumber<T> result(mReal * value, mImaginary * value);
	return result;
}

template class ViComplexNumber<short>;
template class ViComplexNumber<int>;
template class ViComplexNumber<float>;
template class ViComplexNumber<double>;
