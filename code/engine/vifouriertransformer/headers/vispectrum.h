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
	
		ViSpectrum(const bool &autoUpdate = false);
		ViSpectrum(const qint32 &windowSize, const ViAudioFormat &format, const bool &autoUpdate = false);
		ViSpectrum(const qint32 &windowSize, const qint32 &sampleRate, const bool &autoUpdate = false);
		ViSpectrum(const qint32 &windowSize, const ViAudioFormat &format, const ViFrequencyChunk &chunk, const bool &autoUpdate = false);
		ViSpectrum(const qint32 &windowSize, const qint32 &sampleRate, const ViFrequencyChunk &chunk, const bool &autoUpdate = false);
		ViSpectrum(const ViSpectrum<T> &other);

		// If autoUpdate, each time new values are added, the spectrum is updated. Slower, but data is always up-to-date.
		// If not autoUpdate, update() has to be called manually to update the spectrum. Faster, but data must be updated manually.
		void enableAutoUpdate(const bool &enable = true);
		void disableAutoUpdate(const bool &disable = true);

		const ViSpectrumElement<T>& at(const qint32 index) const;
		const ViSpectrumElement<T>& operator[](const qint32 index) const;

		void add(const qint32 index, ViComplexNumber<T> complex);
        void add(const ViFrequencyChunk &chunk);
        void add(const ViFrequencyChunks &chunks);

		qint32 size() const;
		const ViSpectrumElement<T>& maximum() const;
		const ViSpectrumElement<T>& minimum() const;

		void clear();

		void update();

	private:

        void addValue(const qint32 index, ViComplexNumber<T> complex);
        void initialize(qint32 size, qint32 frequency);
		void findRanges();

	private:

		bool mAutoUpdate;
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
