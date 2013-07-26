#ifdef VISPECTRUM_H

#include <float.h>

template <typename T>
ViSpectrum<T>::ViSpectrum(const bool &autoUpdate)
{
	enableAutoUpdate(autoUpdate);
	initialize(0, 0);
}

template <typename T>
ViSpectrum<T>::ViSpectrum(const qint32 &windowSize, const ViAudioFormat &format, const bool &autoUpdate)
{
	enableAutoUpdate(autoUpdate);
	initialize(windowSize + 1, format.sampleRate());
}

template <typename T>
ViSpectrum<T>::ViSpectrum(const qint32 &windowSize, const qint32 &sampleRate, const bool &autoUpdate)
{
	enableAutoUpdate(autoUpdate);
	initialize(windowSize + 1, sampleRate);
}

template <typename T>
ViSpectrum<T>::ViSpectrum(const qint32 &windowSize, const ViAudioFormat &format, const ViFrequencyChunk &chunk, const bool &autoUpdate)
{
	enableAutoUpdate(autoUpdate);
	initialize(windowSize + 1, format.sampleRate());
    add(chunk);
	if(!autoUpdate) update();
}

template <typename T>
ViSpectrum<T>::ViSpectrum(const qint32 &windowSize, const qint32 &sampleRate, const ViFrequencyChunk &chunk, const bool &autoUpdate)
{
	enableAutoUpdate(autoUpdate);
	initialize(windowSize + 1, sampleRate);
	add(chunk);
	if(!autoUpdate) update();
}

template <typename T>
ViSpectrum<T>::ViSpectrum(const ViSpectrum<T> &other)
{
	mAutoUpdate = other.mAutoUpdate;
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
}

template <typename T>
void ViSpectrum<T>::enableAutoUpdate(const bool &enable)
{
	mAutoUpdate = enable;
}

template <typename T>
void ViSpectrum<T>::disableAutoUpdate(const bool &disable)
{
	enableAutoUpdate(!disable);
}

template <typename T>
const ViSpectrumElement<T>& ViSpectrum<T>::at(const qint32 index) const
{
	return mValues[index];
}

template <typename T>
const ViSpectrumElement<T>& ViSpectrum<T>::operator[](const qint32 index) const
{
	return mValues[index];
}

template <typename T>
void ViSpectrum<T>::add(const qint32 index, ViComplexNumber<T> complex)
{
    addValue(index, complex);
	if(mAutoUpdate) update();
}

template <typename T>
void ViSpectrum<T>::add(const ViFrequencyChunk &chunk)
{
	qint32 windowSize = size() - 1;
	addValue(0, ViRealComplex(chunk[0], 0));
    for(int i = 1; i < windowSize; ++i)
    {
		addValue(i, ViRealComplex(chunk[i], -chunk[i + windowSize]));
    }
	addValue(windowSize, ViRealComplex(chunk[windowSize], 0));
	if(mAutoUpdate) update();
}

template <typename T>
void ViSpectrum<T>::add(const ViFrequencyChunks &chunks)
{
	qint32 windowSize = size() - 1;
    for(int c = 0; c < chunks.size(); ++c)
    {
        const ViFrequencyChunk &chunk = chunks[c];
		addValue(0, ViRealComplex(chunk[0], 0));
        for(int i = 1; i < windowSize; ++i)
        {
			addValue(i, ViRealComplex(chunk[i], -chunk[i + windowSize]));
        }
		addValue(windowSize, ViRealComplex(chunk[windowSize], 0));
    }
	if(mAutoUpdate) update();
}

template <typename T>
qint32 ViSpectrum<T>::size() const
{
	return mValues.size();
}

template <typename T>
const ViSpectrumElement<T>& ViSpectrum<T>::maximum() const
{
	return mMaximum;
}

template <typename T>
const ViSpectrumElement<T>& ViSpectrum<T>::minimum() const
{
	return mMinimum;
}

template <typename T>
void ViSpectrum<T>::addValue(const qint32 index, ViComplexNumber<T> complex)
{
    ++mAdditionCounter;
	mRawValues[index].rectangularReference().amplitudeReference() += complex;
	mRawValues[index].polarReference().amplitudeReference() += ViSpectrumElement<T>::toPolar(complex);
}

template <typename T>
void ViSpectrum<T>::initialize(qint32 size, qint32 frequency)
{
	mAdditionCounter = 0;
	mPreviousAdditionCounter = 0;
	mRawValues.clear();
	mRawValues.resize(size);
	mValues.clear();
	mValues.resize(size / 2); // Half real, half imaginary
	qint32 valueSize = mValues.size();
	for(int i = 0; i < valueSize; ++i)
	{
		mValues[i].setFrequencyHertz(frequency * ((i + 1) / T(valueSize)));
		mValues[i].setFrequencyRange(((i + 1) / T(valueSize)));
	}
}

template <typename T>
void ViSpectrum<T>::update()
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
void ViSpectrum<T>::findRanges()
{
	ViComplexNumber<T> valueRectangularAmplitude = mValues[0].rectangular().amplitude();
	ViComplexNumber<T> valuePolarAmplitude = mValues[0].polar().amplitude();
	ViComplexNumber<T> valueRectangularDecibel = mValues[0].rectangular().decibel();
	ViComplexNumber<T> valuePolarDecibel = mValues[0].polar().decibel();

	mMaximum.rectangularReference().amplitudeReference().setReal(valueRectangularAmplitude.real());
	mMinimum.rectangularReference().amplitudeReference().setReal(valueRectangularAmplitude.real());
	mMaximum.rectangularReference().amplitudeReference().setImaginary(valueRectangularAmplitude.imaginary());
	mMinimum.rectangularReference().amplitudeReference().setImaginary(valueRectangularAmplitude.imaginary());
	mMaximum.rectangularReference().decibelReference().setReal(valueRectangularDecibel.real());
	mMinimum.rectangularReference().decibelReference().setReal(valueRectangularDecibel.real());
	mMaximum.rectangularReference().decibelReference().setImaginary(valueRectangularDecibel.imaginary());
	mMinimum.rectangularReference().decibelReference().setImaginary(valueRectangularDecibel.imaginary());
	mMaximum.polarReference().amplitudeReference().setReal(valuePolarAmplitude.real());
	mMinimum.polarReference().amplitudeReference().setReal(valuePolarAmplitude.real());
	mMaximum.polarReference().amplitudeReference().setImaginary(valuePolarAmplitude.imaginary());
	mMinimum.polarReference().amplitudeReference().setImaginary(valuePolarAmplitude.imaginary());
	mMaximum.polarReference().decibelReference().setReal(valuePolarDecibel.real());
	mMinimum.polarReference().decibelReference().setReal(valuePolarDecibel.real());
	mMaximum.polarReference().decibelReference().setImaginary(valuePolarDecibel.imaginary());
	mMinimum.polarReference().decibelReference().setImaginary(valuePolarDecibel.imaginary());

	for(int i = 1; i < mValues.size(); ++i)
	{
		valueRectangularAmplitude = mValues[i].rectangular().amplitude();
		valuePolarAmplitude = mValues[i].polar().amplitude();
		valueRectangularDecibel = mValues[i].rectangular().decibel();
		valuePolarDecibel = mValues[i].polar().decibel();

		if(valueRectangularAmplitude.real() > mMaximum.rectangular().amplitude().real())
		{
			mMaximum.rectangularReference().amplitudeReference().setReal(valueRectangularAmplitude.real());
		}
		else if(valueRectangularAmplitude.real() < mMinimum.rectangular().amplitude().real())
		{
			mMinimum.rectangularReference().amplitudeReference().setReal(valueRectangularAmplitude.real());
		}

		if(valueRectangularAmplitude.imaginary() > mMaximum.rectangular().amplitude().imaginary())
		{
			mMaximum.rectangularReference().amplitudeReference().setImaginary(valueRectangularAmplitude.imaginary());
		}
		else if(valueRectangularAmplitude.imaginary() < mMinimum.rectangular().amplitude().imaginary())
		{
			mMinimum.rectangularReference().amplitudeReference().setImaginary(valueRectangularAmplitude.imaginary());
		}

		if(valueRectangularDecibel.real() > mMaximum.rectangular().decibel().real())
		{
			mMaximum.rectangularReference().decibelReference().setReal(valueRectangularDecibel.real());
		}
		else if(valueRectangularDecibel.real() < mMinimum.rectangular().decibel().real())
		{
			mMinimum.rectangularReference().decibelReference().setReal(valueRectangularDecibel.real());
		}

		if(valueRectangularDecibel.imaginary() > mMaximum.rectangular().decibel().imaginary())
		{
			mMaximum.rectangularReference().decibelReference().setImaginary(valueRectangularDecibel.imaginary());
		}
		else if(valueRectangularDecibel.imaginary() < mMinimum.rectangular().decibel().imaginary())
		{
			mMinimum.rectangularReference().decibelReference().setImaginary(valueRectangularDecibel.imaginary());
		}

		if(valuePolarAmplitude.real() > mMaximum.polar().amplitude().real())
		{
			mMaximum.polarReference().amplitudeReference().setReal(valuePolarAmplitude.real());
		}
		else if(valuePolarAmplitude.real() < mMinimum.polar().amplitude().real())
		{
			mMinimum.polarReference().amplitudeReference().setReal(valuePolarAmplitude.real());
		}

		if(valuePolarAmplitude.imaginary() > mMaximum.polar().amplitude().imaginary())
		{
			mMaximum.polarReference().amplitudeReference().setImaginary(valuePolarAmplitude.imaginary());
		}
		else if(valuePolarAmplitude.imaginary() < mMinimum.polar().amplitude().imaginary())
		{
			mMinimum.polarReference().amplitudeReference().setImaginary(valuePolarAmplitude.imaginary());
		}

		if(valuePolarDecibel.real() > mMaximum.polar().decibel().real())
		{
			mMaximum.polarReference().decibelReference().setReal(valuePolarDecibel.real());
		}
		else if(valuePolarDecibel.real() < mMinimum.polar().decibel().real())
		{
			mMinimum.polarReference().decibelReference().setReal(valuePolarDecibel.real());
		}

		if(valuePolarDecibel.imaginary() > mMaximum.polar().decibel().imaginary())
		{
			mMaximum.polarReference().decibelReference().setImaginary(valuePolarDecibel.imaginary());
		}
		else if(valuePolarDecibel.imaginary() < mMinimum.polar().decibel().imaginary())
		{
			mMinimum.polarReference().decibelReference().setImaginary(valuePolarDecibel.imaginary());
		}


		valueRectangularAmplitude = mValues[i].rectangular().amplitude();
		valuePolarAmplitude = mValues[i].polar().amplitude();
		valueRectangularDecibel = mValues[i].rectangular().decibel();
		valuePolarDecibel = mValues[i].polar().decibel();
	}
}

#endif
