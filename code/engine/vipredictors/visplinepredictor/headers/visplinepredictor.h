#ifndef VISPLINEPREDICTOR_H
#define VISPLINEPREDICTOR_H

#include <vipredictor.h>

class ViSplinePredictor : public ViPredictor
{

	public:

		ViSplinePredictor();
		ViSplinePredictor(const ViSplinePredictor &other);
		~ViSplinePredictor();

		void setDegree(const int &degree);

		void setParameter(const int &number, const qreal &value);

	protected:

		void predict(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount);
		qreal calculateMultiplier(const int &derivative, const int &parameterNumber);

	private:

		int mDegree;

};

#endif
