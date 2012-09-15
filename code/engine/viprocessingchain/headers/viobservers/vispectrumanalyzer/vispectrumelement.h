#ifndef VIELEMENT_H
#define VIELEMENT_H

#include <QMutex>
#include <QVector>
#include <qmath.h>
#include "vicomplexnumber.h"

template <typename T>
class ViSpectrumElementForm
{

	public:

		ViSpectrumElementForm();
		ViSpectrumElementForm(const ViSpectrumElementForm<T> &other);
		ViSpectrumElementForm(ViComplexNumber<T> amplitude, ViComplexNumber<T> decibel);

		void setAmplitude(ViComplexNumber<T> amplitude);
		void setDecibel(ViComplexNumber<T> decibel);

		ViComplexNumber<T>& amplitude();
		ViComplexNumber<T>& decibel();

	private:

		ViComplexNumber<T> mAmplitude;
		ViComplexNumber<T> mDecibel;
};

template <typename T>
class ViSpectrumElement
{

	public:

		ViSpectrumElement();
		ViSpectrumElement(const ViSpectrumElement<T> &other);
		ViSpectrumElement(ViSpectrumElementForm<T> rectangular, ViSpectrumElementForm<T> polar);

		void setFrequencyRange(T frequencyRange);
		void setFrequencyHertz(T frequencyHertz);
		void set(T frequencyRange, T frequencyHertz);

		T frequencyRange();
		T frequencyHertz();

		void setRectangular(ViSpectrumElementForm<T> rectangular);
		void setPolar(ViSpectrumElementForm<T> polar);
		
		ViSpectrumElementForm<T>& rectangular();
		ViSpectrumElementForm<T>& polar();

		static ViComplexNumber<T> toDecibel(ViComplexNumber<T> complex);
		static ViComplexNumber<T> toPolar(ViComplexNumber<T> complex);

	private:

		T mFrequencyRange;
		T mFrequencyHertz;
		
		ViSpectrumElementForm<T> mRectangular;
		ViSpectrumElementForm<T> mPolar;

};

#include "vispectrumelement.cpp"

#endif
