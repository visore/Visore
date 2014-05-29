#ifndef VILAGRANGEPREDICTOR_H
#define VILAGRANGEPREDICTOR_H

#include <vipredictor.h>

class ViLagrangePredictor : public ViPredictor
{

	public:

		ViLagrangePredictor();
		ViLagrangePredictor(const ViLagrangePredictor &other);
		~ViLagrangePredictor();

		bool predict(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount);

		void setParameter(const int &number, const qreal &value);

};

#endif
