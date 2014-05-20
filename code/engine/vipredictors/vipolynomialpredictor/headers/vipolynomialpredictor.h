#ifndef VIPOLYNOMIALPREDICTOR_H
#define VIPOLYNOMIALPREDICTOR_H

#include <vipredictor.h>

class ViPolynomialPredictor : public ViDegreePredictor
{

	public:

		ViPolynomialPredictor();
		ViPolynomialPredictor(const ViPolynomialPredictor &other);
		~ViPolynomialPredictor();

		void setParameter(const int &number, const qreal &value);

	protected:

		void predict(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount);

};

#endif
