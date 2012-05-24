#ifndef VISPECTRUM_H
#define VISPECTRUM_H

#include <QVector>
#include "float.h"
#include "vispectrumelement.h"

template <typename T>
class ViSpectrum
{

	public:
	
		ViSpectrum(qint32 size = 0);

		ViSpectrumElement<T> at(const qint32 index);
		ViSpectrumElement<T> operator[](const qint32 index) const;

		void add(const qint32 index, ViComplexNumber<T> complex);
		ViSpectrumElement<T>& operator[](const qint32 index);

		qint32 size();
		ViSpectrumElement<T> maximum();
		ViSpectrumElement<T> minimum();

		void initialize(qint32 size, qint32 frequency);
		void finalize();

	private:

		void initializeFrequencies(qint32 frequency);
		void findRanges();

	private:

		QVector<ViSpectrumElement<T> > mValues;
		qint32 mAdditionCounter;
		ViSpectrumElement<T> mMaximum;
		ViSpectrumElement<T> mMinimum;

};

typedef ViSpectrum<float>ViFloatSpectrum;
typedef ViSpectrum<double> ViDoubleSpectrum;

#include "../sources/vispectrum.cpp"

#endif
