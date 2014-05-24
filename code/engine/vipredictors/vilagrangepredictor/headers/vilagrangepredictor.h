#ifndef VILAGRANGEPREDICTOR_H
#define VILAGRANGEPREDICTOR_H

#include <vipredictor.h>

class ViLagrangePredictor : public ViPredictor
{

	public:

		ViLagrangePredictor();
		ViLagrangePredictor(const ViLagrangePredictor &other);
		~ViLagrangePredictor();

		void predict(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount);

		void setParameter(const int &number, const qreal &value);

	private:

		int mI;
		int mJ;
		int mK;
		int mX;
		qreal mValue1;
		qreal mValue2;

};

#endif
