#ifndef VISPECTRUMELEMENT_H
#define VISPECTRUMELEMENT_H

#include "math.h"

class ViSpectrumElement
{
	public:
		ViSpectrumElement(qreal frequency = 0, qreal amplitude = 0);
		void setFrequency(qreal frequency);
		void setAmplitude(qreal amplitude);
		qreal frequency();
		qreal amplitude();
		qreal decibel();
		bool clipped();

	private:
		void calculateClipped();
		void calculateDecibel();

	private:
		qreal mFrequency;
		qreal mAmplitude;
		qreal mDecibel;
		bool mClipped;
};

#endif
