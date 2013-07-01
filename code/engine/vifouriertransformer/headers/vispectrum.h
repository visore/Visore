#ifndef VISPECTRUM_H
#define VISPECTRUM_H

#include <QVector>
#include <vispectrumelement.h>
#include <viaudioposition.h>
#include <vichunk.h>

template <typename T>
class ViSpectrum
{

	public:
	
		ViSpectrum();
		ViSpectrum(const qint32 &windowSize, const ViAudioFormat &format);
		ViSpectrum(const qint32 &windowSize, const qint32 &sampleRate);
        ViSpectrum(const qint32 &windowSize, const ViAudioFormat &format, const ViFrequencyChunk &chunk);
		ViSpectrum(const qint32 &windowSize, const qint32 &sampleRate, const ViFrequencyChunk &chunk);
		ViSpectrum(const ViSpectrum<T> &other);

		ViSpectrumElement<T> at(const qint32 index) const;
		ViSpectrumElement<T> operator[](const qint32 index) const;

		void add(const qint32 index, ViComplexNumber<T> complex);
        void add(const ViFrequencyChunk &chunk);
        void add(const ViFrequencyChunks &chunks);

		qint32 size() const;
		ViSpectrumElement<T> maximum() const;
		ViSpectrumElement<T> minimum() const;

		void clear();

	private:

        void addValue(const qint32 index, ViComplexNumber<T> complex);
        void initialize(qint32 size, qint32 frequency);
        void finalize();
		void findRanges();

	private:

		QVector<ViSpectrumElement<T> > mRawValues;
		QVector<ViSpectrumElement<T> > mValues;
		qint32 mAdditionCounter;
		qint32 mPreviousAdditionCounter;
		ViSpectrumElement<T> mMaximum;
		ViSpectrumElement<T> mMinimum;

};

typedef ViSpectrum<short> ViShortSpectrum;
typedef ViSpectrum<float> ViFloatSpectrum;
typedef ViSpectrum<double> ViDoubleSpectrum;
typedef ViDoubleSpectrum ViRealSpectrum;

#include <vispectrum.cpp>

#endif
