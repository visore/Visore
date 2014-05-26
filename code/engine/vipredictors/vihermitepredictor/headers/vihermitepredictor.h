#ifndef VIHERMITEPREDICTOR_H
#define VIHERMITEPREDICTOR_H

#include <vipredictor.h>

class ViHermitePredictor : public ViPredictor
{

	public:

		ViHermitePredictor();
		ViHermitePredictor(const ViHermitePredictor &other);
		~ViHermitePredictor();

		void setParameter(const int &number, const qreal &value);

	protected:

		bool predict(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount);
		bool estimate(const int &degree, ViVector &coefficients, const qreal *samples, const int &size, const qreal &scaling);

		qreal lagrange(const qreal *samples, const int &size, const int &x, const qreal &scaler);
		qreal lagrangeFirstDerivative(const qreal *samples, const int &size, const int &x, const qreal &scaler);

};

#endif
