#include "vispectrum.h"

ViSpectrum::ViSpectrum(int numberOfPoints)
{
	mElements(numberOfPoints);
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

ViSpectrumIterator ViSpectrum::begin()
{
	return mElements.begin();
}

ViSpectrumIterator ViSpectrum::end()
{
	return mElements.end();
}

ViSpectrumConstIterator ViSpectrum::begin() const
{
	return mElements.begin();
}

ViSpectrumConstIterator ViSpectrum::end() const
{
	return mElements.end();
}

void ViSpectrum::initialize()
{
	ViSpectrumConstIterator iterator = begin();
    for (; iterator != end(); ++iterator)
        *iterator = ViSpectrumElement();
}
