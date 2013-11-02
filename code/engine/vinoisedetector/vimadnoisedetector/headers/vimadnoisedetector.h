#ifndef VIMADNOISEDETECTOR_H
#define VIMADNOISEDETECTOR_H

#include <vinoisedetector.h>

// http://www.itl.nist.gov/div898/handbook/eda/section3/eda35h.htm

class ViMadNoiseDetector : public ViNoiseDetector
{

    public:

		ViMadNoiseDetector();
		ViMadNoiseDetector* clone();

	protected:

		void calculateNoise(QQueue<qreal> &samples);

	private:

		int mHalfWindow;

};

#endif

