#include "vifrequencyspectrum.h"

ViFrequencySpectrum::ViFrequencySpectrum(int numPoints)
{
	mElements = QVector<ViSpectrumElement>(numPoints);
}

void ViFrequencySpectrum::reset()
{
	for (ViIterator i = begin(); i != end(); i++)
	{
		*i = ViSpectrumElement();
	}
}

int ViFrequencySpectrum::count() const
{
	return mElements.count();
}

ViFrequencySpectrum::ViSpectrumElement& ViFrequencySpectrum::operator[](int index)
{
	return mElements[index];
}

const ViFrequencySpectrum::ViSpectrumElement& ViFrequencySpectrum::operator[](int index) const
{
	return mElements[index];
}

ViFrequencySpectrum::ViIterator ViFrequencySpectrum::begin()
{
	return mElements.begin();
}

ViFrequencySpectrum::ViIterator ViFrequencySpectrum::end()
{
	return mElements.end();
}

ViFrequencySpectrum::ViConstIterator ViFrequencySpectrum::begin() const
{
	return mElements.begin();
}

ViFrequencySpectrum::ViConstIterator ViFrequencySpectrum::end() const
{
	return mElements.end();
}
