#ifndef VIFREQUENCYSPECTRUM_H
#define VIFREQUENCYSPECTRUM_H

#include <QVector>
#include "float.h"
#include "vifrequencyelement.h"

template <typename T>
class ViFrequencySpectrum
{

	public:
	
		ViFrequencySpectrum(qint32 size = 0);

		ViFrequencyElement<T> at(const qint32 index);
		ViFrequencyElement<T> operator[](const qint32 index) const;

		void add(const qint32 index, ViComplexNumber<T> complex);
		ViFrequencyElement<T>& operator[](const qint32 index);

		qint32 size();
		ViFrequencyElement<T> maximum();
		ViFrequencyElement<T> minimum();

		void initialize(qint32 size, qint32 frequency);
		void finalize();

	private:

		void initializeFrequencies(qint32 frequency);
		void findRanges();

	private:

		QVector<ViFrequencyElement<T> > mValues;
		qint32 mAdditionCounter;
		ViFrequencyElement<T> mMaximum;
		ViFrequencyElement<T> mMinimum;

};

typedef ViFrequencySpectrum<float>ViFloatFrequencySpectrum;
typedef ViFrequencySpectrum<double> ViDoubleFrequencySpectrum;

#include "../sources/vifrequencyspectrum.cpp"

#endif
