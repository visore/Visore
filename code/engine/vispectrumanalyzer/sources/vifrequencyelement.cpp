#ifdef VIFREQUENCYELEMENT_H

template <typename T>
ViFrequencyElementForm<T>::ViFrequencyElementForm()
{
}

template <typename T>
ViFrequencyElementForm<T>::ViFrequencyElementForm(ViComplexNumber<T> amplitude, ViComplexNumber<T> decibel)
{
	mAmplitude = amplitude;
	mDecibel = decibel;
}

template <typename T>
void ViFrequencyElementForm<T>::setAmplitude(ViComplexNumber<T> amplitude)
{
	mAmplitude = amplitude;
}

template <typename T>
void ViFrequencyElementForm<T>::setDecibel(ViComplexNumber<T> decibel)
{
	mDecibel = decibel;
}

template <typename T>
ViComplexNumber<T>& ViFrequencyElementForm<T>::amplitude()
{
	return mAmplitude;
}

template <typename T>
ViComplexNumber<T>& ViFrequencyElementForm<T>::decibel()
{
	return mDecibel;
}

template <typename T>
ViFrequencyElement<T>::ViFrequencyElement()
{
	mFrequencyRange = 0;
	mFrequencyHertz = 0;
}

template <typename T>
ViFrequencyElement<T>::ViFrequencyElement(ViFrequencyElementForm<T> rectangular, ViFrequencyElementForm<T> polar)
{
	mFrequencyRange = 0;
	mFrequencyHertz = 0;
	mRectangular = rectangular;
	mPolar = polar;
}

template <typename T>
void ViFrequencyElement<T>::setFrequencyRange(T frequencyRange)
{
	mFrequencyRange = frequencyRange;
}

template <typename T>
void ViFrequencyElement<T>::setFrequencyHertz(T frequencyHertz)
{
	mFrequencyHertz = frequencyHertz;
}

template <typename T>
void ViFrequencyElement<T>::setFrequency(T frequencyRange, T frequencyHertz)
{
	mFrequencyRange = frequencyRange;
	mFrequencyHertz = frequencyHertz;
}

template <typename T>
T ViFrequencyElement<T>::frequencyRange()
{
	return mFrequencyRange;
}

template <typename T>
T ViFrequencyElement<T>::frequencyHertz()
{
	return mFrequencyHertz;
}

template <typename T>
ViFrequencyElementForm<T>& ViFrequencyElement<T>::rectangular()
{
	return mRectangular;
}

template <typename T>
ViFrequencyElementForm<T>& ViFrequencyElement<T>::polar()
{
	return mPolar;
}

template <typename T>
void ViFrequencyElement<T>::setRectangular(ViFrequencyElementForm<T> rectangular)
{
	mRectangular = rectangular;
}

template <typename T>
void ViFrequencyElement<T>::setPolar(ViFrequencyElementForm<T> polar)
{
	mPolar = polar;
}

template <typename T>
ViComplexNumber<T> ViFrequencyElement<T>::toDecibel(ViComplexNumber<T> complex)
{
	return ViComplexNumber<T>(20 * log10(complex.real()), 20 * log10(complex.imaginary()));
}

template <typename T>
ViComplexNumber<T> ViFrequencyElement<T>::toPolar(ViComplexNumber<T> complex)
{
	return ViComplexNumber<T>(
		qSqrt(complex.real() * complex.real() + complex.imaginary() * complex.imaginary()),
		qAtan(complex.imaginary() / complex.real())
	);
}

#endif
