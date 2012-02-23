#include "vispectrum.h"

ViSpectrum::ViSpectrum(int numberOfPoints)
{
	mElements = QVector<ViSpectrumElement>(numberOfPoints);
	initialize();
}

void ViSpectrum::reset()
{
	initialize();
}

int ViSpectrum::size()
{
	mElements.count();
}

ViSpectrumElement& ViSpectrum::operator[](int index)
{
	return mElements[index];
}

const ViSpectrumElement& ViSpectrum::operator[](int index) const
{
	return mElements[index];
}

ViSpectrum::ViSpectrumIterator ViSpectrum::begin()
{
	return mElements.begin();
}

ViSpectrum::ViSpectrumIterator ViSpectrum::end()
{
	return mElements.end();
}

ViSpectrum::ViSpectrumConstIterator ViSpectrum::begin() const
{
	return mElements.begin();
}

ViSpectrum::ViSpectrumConstIterator ViSpectrum::end() const
{
	return mElements.end();
}

void ViSpectrum::initialize()
{
	ViSpectrumIterator iterator = begin();
    for (; iterator != end(); ++iterator)
        *iterator = ViSpectrumElement();
}
