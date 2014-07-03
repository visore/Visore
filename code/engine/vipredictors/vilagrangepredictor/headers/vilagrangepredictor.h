#ifndef VILAGRANGEPREDICTOR_H
#define VILAGRANGEPREDICTOR_H

#include <vipredictor.h>

class ViLagrangePredictor : public ViPredictor
{

	public:

		ViLagrangePredictor();
		ViLagrangePredictor(const ViLagrangePredictor &other);
		~ViLagrangePredictor();

		void setParameter(const int &number, const qreal &value);
		bool validParameters();

		bool predict(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, ViError *error = NULL, const int &channel = 0);

};

#endif

