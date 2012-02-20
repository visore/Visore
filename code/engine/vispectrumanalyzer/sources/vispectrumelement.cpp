#include "vispectrumelement.h"

ViSpectrumElement::ViSpectrumElement(qreal frequency, qreal amplitude)
{
	mFrequency = frequency;
	mAmplitude = amplitude;
	calculateClipped();
	calculateDecibel();
}

void ViSpectrumElement::setFrequency(qreal frequency)
{
	mFrequency = frequency;
}

void ViSpectrumElement::setAmplitude(qreal amplitude)
{
	mAmplitude = amplitude;
	calculateClipped();
	calculateDecibel();
}

qreal ViSpectrumElement::frequency()
{
	return mFrequency;
}

qreal ViSpectrumElement::amplitude()
{
	return mAmplitude;
}

qreal ViSpectrumElement::decibel()
{
	return mDecibel;
}

bool ViSpectrumElement::clipped()
{
	return mClipped;
}

void ViSpectrumElement::calculateClipped()
{
	mClipped = (mAmplitude > 1.0);
	mAmplitude = qMax(qreal(0.0), mAmplitude);
	mAmplitude = qMin(qreal(1.0), mAmplitude);
}

void ViSpectrumElement::calculateDecibel()
{
	mDecibel = 10.0 * log(mAmplitude);
}
