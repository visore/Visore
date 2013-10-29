#ifndef VIPREDICTIONNOISEDETECTOR_H
#define VIPREDICTIONNOISEDETECTOR_H

#include <vinoisedetector.h>
#include <vimatrix.h>

class ViPredictionNoiseDetector : public ViNoiseDetector
{

    public:

		ViPredictionNoiseDetector(const int &degree = 6);
		ViPredictionNoiseDetector* clone();

		void setDegree(const int &degree);
		int degree();

	protected:

		void calculateNoise(QQueue<qreal> &samples);

	private:

		int mDegree;
		ViMatrix mMatrix;
		QList<qreal> mPowers;

};

#endif

