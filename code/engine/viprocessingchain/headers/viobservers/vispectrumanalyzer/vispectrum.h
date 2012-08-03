#ifndef VISPECTRUM_H
#define VISPECTRUM_H

#include <QVector>
#include "float.h"
#include "vispectrumelement.h"
#include "viaudioposition.h"

template <typename T>
class ViSpectrum
{

	public:
	
		ViSpectrum();

		ViSpectrumElement<T> at(const qint32 index);
		ViSpectrumElement<T> operator[](const qint32 index) const;

		void add(const qint32 index, ViComplexNumber<T> complex);

		qint32 size();
		ViSpectrumElement<T> maximum();
		ViSpectrumElement<T> minimum();

		void initialize(qint32 size, qint32 frequency);
		void finalize();

		void clear();

		ViAudioPosition interval();
		void setInterval(ViAudioPosition interval);

	private:

		void findRanges();

	private:

		QVector<ViSpectrumElement<T> > mRawValues;
		QVector<ViSpectrumElement<T> > mValues;
		qint32 mAdditionCounter;
		qint32 mPreviousAdditionCounter;
		ViSpectrumElement<T> mMaximum;
		ViSpectrumElement<T> mMinimum;
		ViAudioPosition mInterval;

};

typedef ViSpectrum<short> ViShortSpectrum;
typedef ViSpectrum<float> ViFloatSpectrum;
typedef ViSpectrum<double> ViDoubleSpectrum;
typedef ViDoubleSpectrum ViRealSpectrum;

#include "vispectrum.cpp"

#endif
