#ifndef VIZSCORENOISEDETECTOR_H
#define VIZSCORENOISEDETECTOR_H

#include <vinoisedetector.h>

// http://en.wikipedia.org/wiki/Standard_score
// http://www.itl.nist.gov/div898/handbook/eda/section3/eda35h.htm
// http://d-scholarship.pitt.edu/7948/1/Seo.pdf
// http://habcam.whoi.edu/HabCamData/HAB/processed/Outlier%20Methods_external.pdf

class ViZscoreNoiseDetector : public ViNoiseDetector
{

    public:

		ViZscoreNoiseDetector();
		ViZscoreNoiseDetector* clone();

		void setWindowSize(const int &size);
		void setParameters(const qreal &param1);

	protected:

		void calculateNoise(QQueue<qreal> &samples);

	private:

		int mWindowSize;
		int mHalfWindow;

};

#endif

