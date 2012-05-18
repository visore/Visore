#include "vifrequencyspectrum.h"

qreal ViFrequencySpectrum::real(const qint32 index)
{
	qreal result = mData[index].real();
}

qreal ViFrequencySpectrum::imaginary(const qint32 index)
{
	return mData[index].imaginary();
}

ViComplexFloat ViFrequencySpectrum::at(const qint32 index)
{
	return mData[index];
}

ViComplexFloat ViFrequencySpectrum::operator[](const qint32 index) const
{
	return mData[index];
}

void ViFrequencySpectrum::append(ViComplexFloat complex)
{
	mData.append(complex);
}

void ViFrequencySpectrum::setReal(const qint32 index, qreal real)
{
	mData[index].setReal(real);
}

void ViFrequencySpectrum::setImaginary(const qint32 index, qreal imaginary)
{
	mData[index].setImaginary(imaginary);
}

void ViFrequencySpectrum::set(const qint32 index, qreal real, qreal imaginary)
{
	mData[index].set(real, imaginary);
}

void ViFrequencySpectrum::set(const qint32 index, ViComplexFloat complex)
{
	mData[index].set(complex);
}

ViComplexFloat& ViFrequencySpectrum::operator[](const qint32 index)
{
	return mData[index];
}

void ViFrequencySpectrum::lock()
{
	mMutex.lock();
}

void ViFrequencySpectrum::unlock()
{
	mMutex.unlock();
}
