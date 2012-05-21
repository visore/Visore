#ifdef VIFREQUENCYSPECTRUM_H

template <typename T>
ViFrequencySpectrum<T>::ViFrequencySpectrum(qint32 size)
{
}

template <typename T>
ViFrequencyElement<T> ViFrequencySpectrum<T>::at(const qint32 index)
{
	return mValues[index];
}

template <typename T>
ViFrequencyElement<T> ViFrequencySpectrum<T>::operator[](const qint32 index) const
{
	return mValues[index];
}

template <typename T>
void ViFrequencySpectrum<T>::add(const qint32 index, ViComplexNumber<T> complex)
{
	++mAdditionCounter;
	mValues[index].rectangular().amplitude() += complex;
	mValues[index].polar().amplitude() += ViFrequencyElement<T>::toPolar(complex);
}

template <typename T>
ViFrequencyElement<T>& ViFrequencySpectrum<T>::operator[](const qint32 index)
{
	return mValues[index];
}

template <typename T>
qint32 ViFrequencySpectrum<T>::size()
{
	return mValues.size();
}

template <typename T>
ViFrequencyElement<T> ViFrequencySpectrum<T>::maximum()
{
	return mMaximum;
}

template <typename T>
ViFrequencyElement<T> ViFrequencySpectrum<T>::minimum()
{
	return mMinimum;
}

template <typename T>
void ViFrequencySpectrum<T>::initialize(qint32 size, qint32 frequency)
{
	mAdditionCounter = 0;
	mValues.clear();
	mValues.resize(size);
	initializeFrequencies(frequency);
}

template <typename T>
void ViFrequencySpectrum<T>::finalize()
{
	for(int i = 0; i < mValues.size(); ++i)
	{
		mValues[i].rectangular().amplitude() /= T(mAdditionCounter);
		mValues[i].polar().amplitude() /= T(mAdditionCounter);
		mValues[i].rectangular().setDecibel(ViFrequencyElement<T>::toDecibel(mValues[i].rectangular().amplitude()));
		mValues[i].polar().setDecibel(ViFrequencyElement<T>::toDecibel(mValues[i].polar().amplitude()));
	}

	findRanges();
}

template <typename T>
void ViFrequencySpectrum<T>::initializeFrequencies(qint32 frequency)
{
	qint32 size = mValues.size();
	for(int i = 0; i < size; ++i)
	{
		mValues[i].setFrequencyHertz(frequency * ((i + 1) / T(size)));
		mValues[i].setFrequencyRange(0.5 * ((i + 1) / T(size)));
	}
}

template <typename T>
void ViFrequencySpectrum<T>::findRanges()
{
	mMaximum.setPolar(ViFrequencyElementForm<T>(ViComplexNumber<T>(FLT_MIN, FLT_MIN), ViComplexNumber<T>(FLT_MIN, FLT_MIN)));
	mMaximum.setRectangular(ViFrequencyElementForm<T>(ViComplexNumber<T>(FLT_MIN, FLT_MIN), ViComplexNumber<T>(FLT_MIN, FLT_MIN)));
	mMinimum.setPolar(ViFrequencyElementForm<T>(ViComplexNumber<T>(FLT_MAX, FLT_MAX), ViComplexNumber<T>(FLT_MAX, FLT_MAX)));
	mMinimum.setRectangular(ViFrequencyElementForm<T>(ViComplexNumber<T>(FLT_MAX, FLT_MAX), ViComplexNumber<T>(FLT_MAX, FLT_MAX)));

	ViComplexNumber<T> valueRectangularAmplitude;
	ViComplexNumber<T> valuePolarAmplitude;
	ViComplexNumber<T> valueRectangularDecibel;
	ViComplexNumber<T> valuePolarDecibel;


	for(int i = 0; i < mValues.size(); ++i)
	{
		valueRectangularAmplitude = mValues[i].rectangular().amplitude();
		valuePolarAmplitude = mValues[i].polar().amplitude();
		valueRectangularDecibel =mValues[i].rectangular().decibel();
		valuePolarDecibel = mValues[i].polar().decibel();

		if(valueRectangularAmplitude.real() > mMaximum.rectangular().amplitude().real())
		{
			mMaximum.rectangular().amplitude().setReal(valueRectangularAmplitude.real());
		}
		else if(valueRectangularAmplitude.real() < mMinimum.rectangular().amplitude().real())
		{
			mMinimum.rectangular().amplitude().setReal(valueRectangularAmplitude.real());
		}

		if(valueRectangularAmplitude.imaginary() > mMaximum.rectangular().amplitude().imaginary())
		{
			mMaximum.rectangular().amplitude().setImaginary(valueRectangularAmplitude.imaginary());
		}
		else if(valueRectangularAmplitude.imaginary() < mMinimum.rectangular().amplitude().imaginary())
		{
			mMinimum.rectangular().amplitude().setImaginary(valueRectangularAmplitude.imaginary());
		}

		if(valueRectangularDecibel.real() > mMaximum.rectangular().decibel().real())
		{
			mMaximum.rectangular().decibel().setReal(valueRectangularDecibel.real());
		}
		else if(valueRectangularDecibel.real() < mMinimum.rectangular().decibel().real())
		{
			mMinimum.rectangular().decibel().setReal(valueRectangularDecibel.real());
		}

		if(valueRectangularDecibel.imaginary() > mMaximum.rectangular().decibel().imaginary())
		{
			mMaximum.rectangular().decibel().setImaginary(valueRectangularDecibel.imaginary());
		}
		else if(valueRectangularDecibel.imaginary() < mMinimum.rectangular().decibel().imaginary())
		{
			mMinimum.rectangular().decibel().setImaginary(valueRectangularDecibel.imaginary());
		}

		if(valuePolarAmplitude.real() > mMaximum.polar().amplitude().real())
		{
			mMaximum.polar().amplitude().setReal(valuePolarAmplitude.real());
		}
		else if(valuePolarAmplitude.real() < mMinimum.polar().amplitude().real())
		{
			mMinimum.polar().amplitude().setReal(valuePolarAmplitude.real());
		}

		if(valuePolarAmplitude.imaginary() > mMaximum.polar().amplitude().imaginary())
		{
			mMaximum.polar().amplitude().setImaginary(valuePolarAmplitude.imaginary());
		}
		else if(valuePolarAmplitude.imaginary() < mMinimum.polar().amplitude().imaginary())
		{
			mMinimum.polar().amplitude().setImaginary(valuePolarAmplitude.imaginary());
		}

		if(valuePolarDecibel.real() > mMaximum.polar().decibel().real())
		{
			mMaximum.polar().decibel().setReal(valuePolarDecibel.real());
		}
		else if(valuePolarDecibel.real() < mMinimum.polar().decibel().real())
		{
			mMinimum.polar().decibel().setReal(valuePolarDecibel.real());
		}

		if(valuePolarDecibel.imaginary() > mMaximum.polar().decibel().imaginary())
		{
			mMaximum.polar().decibel().setImaginary(valuePolarDecibel.imaginary());
		}
		else if(valuePolarDecibel.imaginary() < mMinimum.polar().decibel().imaginary())
		{
			mMinimum.polar().decibel().setImaginary(valuePolarDecibel.imaginary());
		}
	}
}

#endif
