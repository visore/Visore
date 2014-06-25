#ifndef VIMADNOISEDETECTOR_H
#define VIMADNOISEDETECTOR_H

#include <vinoisedetector.h>

// http://www.itl.nist.gov/div898/handbook/eda/section3/eda35h.htm

class ViMadNoiseDetector : public ViNoiseDetector
{

    public:

		ViMadNoiseDetector();
		ViMadNoiseDetector(const ViMadNoiseDetector &other);
		~ViMadNoiseDetector();

		void setWindowSize(const int &size);

		bool validParameters();

	protected:

		void initialize();
		void detect(QVector<qreal> &samples, QVector<qreal> &noise);

	private:

		int mWindowSize;
		int mHalfWindowSize;
		qreal *mWindow;

};

#endif


