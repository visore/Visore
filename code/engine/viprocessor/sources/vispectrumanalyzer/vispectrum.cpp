#ifdef VISPECTRUM_H

template <typename T>
ViSpectrum<T>::ViSpectrum()
{
}

template <typename T>
ViSpectrum<T>::ViSpectrum(const ViSpectrum<T> &other)
{
	for(int i = 0; i < other.mRawValues.size(); ++i)
	{
		mRawValues.push_back(ViSpectrumElement<T>(other.mRawValues[i]));
	}
	for(int i = 0; i < other.mValues.size(); ++i)
	{
		mValues.push_back(ViSpectrumElement<T>(other.mValues[i]));
	}
	mAdditionCounter = other.mAdditionCounter;
	mPreviousAdditionCounter = other.mPreviousAdditionCounter;
	mMaximum = ViSpectrumElement<T>(other.mMaximum);
	mMinimum = ViSpectrumElement<T>(other.mMinimum);
	mInterval = other.mInterval;
}

template <typename T>
ViSpectrumElement<T> ViSpectrum<T>::at(const qint32 index)
{
	return mValues[index];
}

template <typename T>
ViSpectrumElement<T> ViSpectrum<T>::operator[](const qint32 index) const
{
	return mValues[index];
}

template <typename T>
void ViSpectrum<T>::add(const qint32 index, ViComplexNumber<T> complex)
{
	++mAdditionCounter;
	mRawValues[index].rectangular().amplitude() += complex;
	mRawValues[index].polar().amplitude() += ViSpectrumElement<T>::toPolar(complex);
}

template <typename T>
qint32 ViSpectrum<T>::size()
{
	return mValues.size();
}

template <typename T>
ViSpectrumElement<T> ViSpectrum<T>::maximum()
{
	return mMaximum;
}

template <typename T>
ViSpectrumElement<T> ViSpectrum<T>::minimum()
{
	return mMinimum;
}

template <typename T>
void ViSpectrum<T>::initialize(qint32 size, qint32 frequency)
{
	mAdditionCounter = 0;
	mPreviousAdditionCounter = 0;
	mRawValues.clear();
	mRawValues.resize(size);
	mValues.clear();
	mValues.resize(size / 2); // Since we use real FFT, we'll have a mirror image, hence we only need half of the N/2 spectrum
	qint32 valueSize = mValues.size();
	for(int i = 0; i < valueSize; ++i)
	{
		mValues[i].setFrequencyHertz(frequency * ((i + 1) / T(valueSize)));
		mValues[i].setFrequencyRange(0.5 * ((i + 1) / T(valueSize)));
	}
}

template <typename T>
void ViSpectrum<T>::finalize()
{
	if(mPreviousAdditionCounter != mAdditionCounter && mAdditionCounter != 0)
	{
		ViComplexNumber<T> rectangularAmplitude, rectangularDecibel, polarAmplitude, polarDecibel;
		for(int i = 0; i < mValues.size(); ++i)
		{
			rectangularAmplitude = mRawValues[i].rectangular().amplitude() / T(mAdditionCounter);
			rectangularDecibel = ViSpectrumElement<T>::toDecibel(rectangularAmplitude);
			polarAmplitude = mRawValues[i].polar().amplitude() / T(mAdditionCounter);
			polarDecibel = ViSpectrumElement<T>::toDecibel(polarAmplitude);
			mValues[i].setRectangular(ViSpectrumElementForm<T>(rectangularAmplitude, rectangularDecibel));
			mValues[i].setPolar(ViSpectrumElementForm<T>(polarAmplitude, polarDecibel));

		}
		findRanges();
		mPreviousAdditionCounter = mAdditionCounter;
	}
}

template <typename T>
void ViSpectrum<T>::clear()
{
	mRawValues.clear();
	mValues.clear();
	mAdditionCounter = 0;
	mPreviousAdditionCounter = 0;
}

template <typename T>
ViAudioPosition ViSpectrum<T>::interval()
{
	return mInterval;
}

template <typename T>
void ViSpectrum<T>::setInterval(ViAudioPosition interval)
{
	mInterval = interval;
}

template <typename T>
void ViSpectrum<T>::findRanges()
{
	mMaximum.setPolar(ViSpectrumElementForm<T>(ViComplexNumber<T>(-FLT_MAX, -FLT_MAX), ViComplexNumber<T>(-FLT_MAX, -FLT_MAX)));
	mMaximum.setRectangular(ViSpectrumElementForm<T>(ViComplexNumber<T>(-FLT_MAX, -FLT_MAX), ViComplexNumber<T>(-FLT_MAX, -FLT_MAX)));
	mMinimum.setPolar(ViSpectrumElementForm<T>(ViComplexNumber<T>(FLT_MAX, FLT_MAX), ViComplexNumber<T>(FLT_MAX, FLT_MAX)));
	mMinimum.setRectangular(ViSpectrumElementForm<T>(ViComplexNumber<T>(FLT_MAX, FLT_MAX), ViComplexNumber<T>(FLT_MAX, FLT_MAX)));

	ViComplexNumber<T> valueRectangularAmplitude;
	ViComplexNumber<T> valuePolarAmplitude;
	ViComplexNumber<T> valueRectangularDecibel;
	ViComplexNumber<T> valuePolarDecibel;

	for(int i = 0; i < mValues.size(); ++i)
	{
		valueRectangularAmplitude = mValues[i].rectangular().amplitude();
		valuePolarAmplitude = mValues[i].polar().amplitude();
		valueRectangularDecibel = mValues[i].rectangular().decibel();
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
