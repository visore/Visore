#ifndef VIPOLYNOMIALPREDICTOR_H
#define VIPOLYNOMIALPREDICTOR_H

#include <vipredictor.h>

class ViPolynomialPredictor : public ViDegreePredictor
{

	public:

		ViPolynomialPredictor();
		ViPolynomialPredictor(const ViPolynomialPredictor &other);
		~ViPolynomialPredictor();

		void setParameters(const qreal &parameter1, const qreal &parameter2);

	protected:

		void predict(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount);

};

#endif
