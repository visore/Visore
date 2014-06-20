/*#ifndef VIMADNOISEDETECTOR_H
#define VIMADNOISEDETECTOR_H

#include <vinoisedetector.h>

// http://www.itl.nist.gov/div898/handbook/eda/section3/eda35h.htm

class ViMadNoiseDetector : public ViNoiseDetector
{

    public:

		ViMadNoiseDetector();
		ViMadNoiseDetector(const ViMadNoiseDetector &other);
		~ViMadNoiseDetector();
		ViMadNoiseDetector* clone();

		void setWindowSize(const int &size);
		void setParameters(const qreal &param1);

	protected:

		void calculateNoise(QQueue<qreal> &samples);

	private:

		int mWindowSize;
		int mHalfWindowSize;
		qreal *mWindow;

};

#endif

*/
