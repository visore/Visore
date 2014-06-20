/*#ifndef VIPREDICTIONNOISEDETECTOR_H
#define VIPREDICTIONNOISEDETECTOR_H

#include <vinoisedetector.h>
#include <vimatrix.h>

class ViPredictionNoiseDetector : public ViNoiseDetector
{

    public:

		ViPredictionNoiseDetector(const int &degree = 2);
		ViPredictionNoiseDetector* clone();

		void setDegree(const int &degree);
		int degree();

		QString name(QString replace = "", bool spaced = false);

	protected:

		void calculateNoise(QQueue<qreal> &samples);

	private:

		int mDegree;
		ViMatrix mMatrix;
		QList<qreal> mPowers;

};

#endif

*/
