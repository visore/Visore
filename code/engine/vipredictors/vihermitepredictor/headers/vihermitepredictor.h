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
		bool validParameters();

		ViHermitePredictor* clone();

	protected:

		bool predict(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, ViError *modelError = NULL);

		void calculate(const qreal *x, const qreal *y, const qreal *derivatives, const int &size, qreal *output, const int &outputSize, const int &startX, const qreal &scaling);
		qreal calculateLagrange(const qreal *x, const int &size, const qreal &theX, const int &j);
		qreal calculateLagrangeDerivative1(const qreal *x, const int &size, const qreal &theX, const int &j);


};

#endif

