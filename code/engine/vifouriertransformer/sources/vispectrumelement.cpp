#ifdef VISPECTRUMELEMENT_H

template <typename T>
ViSpectrumElementForm<T>::ViSpectrumElementForm()
{
}

template <typename T>
ViSpectrumElementForm<T>::ViSpectrumElementForm(const ViSpectrumElementForm<T> &other)
{
	mAmplitude = ViComplexNumber<T>(other.mAmplitude);
	mDecibel = ViComplexNumber<T>(other.mDecibel);
}

template <typename T>
ViSpectrumElementForm<T>::ViSpectrumElementForm(ViComplexNumber<T> amplitude, ViComplexNumber<T> decibel)
{
	mAmplitude = amplitude;
	mDecibel = decibel;
}

template <typename T>
void ViSpectrumElementForm<T>::setAmplitude(ViComplexNumber<T> amplitude)
{
	mAmplitude = amplitude;
}

template <typename T>
void ViSpectrumElementForm<T>::setDecibel(ViComplexNumber<T> decibel)
{
	mDecibel = decibel;
}

template <typename T>
ViComplexNumber<T>& ViSpectrumElementForm<T>::amplitude()
{
	return mAmplitude;
}

template <typename T>
ViComplexNumber<T>& ViSpectrumElementForm<T>::decibel()
{
	return mDecibel;
}

template <typename T>
ViSpectrumElement<T>::ViSpectrumElement()
{
	mFrequencyRange = 0;
	mFrequencyHertz = 0;
}

template <typename T>
ViSpectrumElement<T>::ViSpectrumElement(const ViSpectrumElement<T> &other)
{
	mFrequencyRange = other.mFrequencyRange;
	mFrequencyHertz = other.mFrequencyHertz;
	mRectangular = ViSpectrumElementForm<T>(other.mRectangular);
	mPolar = ViSpectrumElementForm<T>(other.mPolar);
}

template <typename T>
ViSpectrumElement<T>::ViSpectrumElement(ViSpectrumElementForm<T> rectangular, ViSpectrumElementForm<T> polar)
{
	mFrequencyRange = 0;
	mFrequencyHertz = 0;
	mRectangular = rectangular;
	mPolar = polar;
}

template <typename T>
void ViSpectrumElement<T>::setFrequencyRange(T frequencyRange)
{
	mFrequencyRange = frequencyRange;
}

template <typename T>
void ViSpectrumElement<T>::setFrequencyHertz(T frequencyHertz)
{
	mFrequencyHertz = frequencyHertz;
}

template <typename T>
void ViSpectrumElement<T>::set(T frequencyRange, T frequencyHertz)
{
	mFrequencyRange = frequencyRange;
	mFrequencyHertz = frequencyHertz;
}

template <typename T>
T ViSpectrumElement<T>::frequencyRange()
{
	return mFrequencyRange;
}

template <typename T>
T ViSpectrumElement<T>::frequencyHertz()
{
	return mFrequencyHertz;
}

template <typename T>
ViSpectrumElementForm<T>& ViSpectrumElement<T>::rectangular()
{
	return mRectangular;
}

template <typename T>
ViSpectrumElementForm<T>& ViSpectrumElement<T>::polar()
{
	return mPolar;
}

template <typename T>
void ViSpectrumElement<T>::setRectangular(ViSpectrumElementForm<T> rectangular)
{
	mRectangular = rectangular;
}

template <typename T>
void ViSpectrumElement<T>::setPolar(ViSpectrumElementForm<T> polar)
{
	mPolar = polar;
}

template <typename T>
ViComplexNumber<T> ViSpectrumElement<T>::toDecibel(ViComplexNumber<T> complex)
{
	return ViComplexNumber<T>(20 * log10(complex.real()), 20 * log10(complex.imaginary()));
}

template <typename T>
ViComplexNumber<T> ViSpectrumElement<T>::toPolar(ViComplexNumber<T> complex)
{
	return ViComplexNumber<T>(
		qSqrt(complex.real() * complex.real() + complex.imaginary() * complex.imaginary()),
		qAtan(complex.imaginary() / complex.real())
	);
}

#endif
