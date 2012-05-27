#ifndef VIFREQUENCYELEMENT_H
#define VIFREQUENCYELEMENT_H

#include <QMutex>
#include <QVector>
#include <qmath.h>
#include "vicomplexnumber.h"

template <typename T>
class ViFrequencyElementForm
{

	public:

		ViFrequencyElementForm();
		ViFrequencyElementForm(ViComplexNumber<T> amplitude, ViComplexNumber<T> decibel);

		void setAmplitude(ViComplexNumber<T> amplitude);
		void setDecibel(ViComplexNumber<T> decibel);

		ViComplexNumber<T>& amplitude();
		ViComplexNumber<T>& decibel();

	private:

		ViComplexNumber<T> mAmplitude;
		ViComplexNumber<T> mDecibel;
};

template <typename T>
class ViFrequencyElement
{

	public:

		ViFrequencyElement();
		ViFrequencyElement(ViFrequencyElementForm<T> rectangular, ViFrequencyElementForm<T> polar);

		void setFrequencyRange(T frequencyRange);
		void setFrequencyHertz(T frequencyHertz);
		void setFrequency(T frequencyRange, T frequencyHertz);

		T frequencyRange();
		T frequencyHertz();

		void setRectangular(ViFrequencyElementForm<T> rectangular);
		void setPolar(ViFrequencyElementForm<T> polar);
		
		ViFrequencyElementForm<T>& rectangular();
		ViFrequencyElementForm<T>& polar();

		static ViComplexNumber<T> toDecibel(ViComplexNumber<T> complex);
		static ViComplexNumber<T> toPolar(ViComplexNumber<T> complex);

	private:

		T mFrequencyRange;
		T mFrequencyHertz;
		
		ViFrequencyElementForm<T> mRectangular;
		ViFrequencyElementForm<T> mPolar;

};

#include "../sources/vifrequencyelement.cpp"

#endif
