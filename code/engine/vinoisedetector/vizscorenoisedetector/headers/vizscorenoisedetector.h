#ifndef VIZSCORENOISEDETECTOR_H
#define VIZSCORENOISEDETECTOR_H

#include <vinoisedetector.h>

// http://en.wikipedia.org/wiki/Standard_score
// http://www.itl.nist.gov/div898/handbook/eda/section3/eda35h.htm
// http://d-scholarship.pitt.edu/7948/1/Seo.pdf
// http://habcam.whoi.edu/HabCamData/HAB/processed/Outlier%20Methods_external.pdf

class ViZscoreNoiseDetector : public ViNoiseDetector
{

	Q_OBJECT

	protected slots:

		void changeParameter(QString name, qreal value);

    public:

		ViZscoreNoiseDetector();

		void setWindowSize(const int &size);

		bool validParameters();

	protected:

		void detect(QVector<qreal> &samples, QVector<qreal> &noise, const int &channel = 0);

	private:

		int mWindowSize;
		int mHalfWindow;

};

#endif
