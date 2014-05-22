#ifndef VIARMAPREDICTOR_H
#define VIARMAPREDICTOR_H

#include <vipredictor.h>
#include <vigretl.h>

class ViArmaPredictor : public ViPredictor
{

	public:

		ViArmaPredictor();
		ViArmaPredictor(const ViArmaPredictor &other);
		~ViArmaPredictor();

		void setWindowSize(const int &size);
		void setDegree(const ViGretl::Type &type, const int &degree);

		void setParameter(const int &number, const qreal &value);

	protected:

		void predict(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount);

	private:

		ViGretl *mGretl;


};

#endif
