#ifndef VISPECTRUM_H
#define VISPECTRUM_H

#include "vispectrumelement.h"
#include <QVector>

class ViSpectrum
{
	public:
		typedef QVector<ViSpectrumElement>::iterator ViSpectrumIterator;
		typedef QVector<ViSpectrumElement>::const_iterator ViSpectrumConstIterator;

		ViSpectrum(int numberOfPoints = 0);
		void reset();
		int size();
		ViSpectrumElement& operator[](int index);
		const ViSpectrumElement& operator[](int index) const;
		ViSpectrumIterator begin();
		ViSpectrumIterator end();
		ViSpectrumConstIterator begin() const;
		ViSpectrumConstIterator end() const;

	private:
		void initialize();

	private:
		QVector<ViSpectrumElement> mElements;
};

#endif
