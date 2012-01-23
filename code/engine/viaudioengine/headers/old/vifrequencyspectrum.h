/*
This is an adaption of the Qt Spectrum demo.
*/

#ifndef VIFREQUENCYSPECTRUM_H
#define VIFREQUENCYSPECTRUM_H

#include <QVector>

/**
 * Represents a frequency spectrum as a series of elements, each of which
 * consists of a frequency, an amplitude and a phase.
 */
class ViFrequencySpectrum
{
	public:
		struct ViSpectrumElement
		{
			ViSpectrumElement()
			{
				mFrequency = 0.0;
				mAmplitude = 0.0;
				mPhase = 0.0;
				mClipped = false;
			}

			//Frequency in Hertz
			qreal mFrequency;

			//Amplitude in range [0.0, 1.0]
			qreal mAmplitude;

			//Phase in range [0.0, 2*PI]
			qreal mPhase;

			//Indicates whether value has been clipped during spectrum analysis
			bool mClipped;
		};
		typedef QVector<ViSpectrumElement>::iterator ViIterator;
		typedef QVector<ViSpectrumElement>::const_iterator ViConstIterator;

		ViFrequencySpectrum(int numPoints = 0);
		void reset();
		int count() const;
		ViSpectrumElement& operator[](int index);
		const ViSpectrumElement& operator[](int index) const;
		ViIterator begin();
		ViIterator end();
		ViConstIterator begin() const;
		ViConstIterator end() const;

	private:
		QVector<ViSpectrumElement> mElements;

};

#endif
