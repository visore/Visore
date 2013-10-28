#ifndef VIPREDICTIONNOISEDETECTOR_H
#define VIPREDICTIONNOISEDETECTOR_H

#include <vinoisedetector.h>
#include <vimatrix.h>

class ViPredictionNoiseDetector : public ViNoiseDetector
{

    public:

		ViPredictionNoiseDetector(const int &degree = 1);
		ViPredictionNoiseDetector* clone();

		void setDegree(const int &degree);
		int degree();

	protected:

		void calculateNoise(const ViSampleChunk &samples);

	private:

		int mDegree;
		QQueue<qreal> mCache;

		ViMatrix mMatrix;
		QList<qreal> mPowers;

};

#endif
