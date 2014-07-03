#ifndef VIGARCHPREDICTOR_H
#define VIGARCHPREDICTOR_H

#include <vipredictor.h>
#include <vigretl.h>

class ViGarchPredictor : public ViPredictor
{

	public:

		ViGarchPredictor();
		ViGarchPredictor(const ViGarchPredictor &other);
		~ViGarchPredictor();

		void setWindowSize(const int &size);
		void setDegree(const ViGretl::Type &type, const int &degree);

		void setParameter(const int &number, const qreal &value);

		bool predict(const qreal *samples, const int &size, qreal *predictedSamples, const int &predictionCount, ViError *error = NULL, const int &channel = 0);

	private:

		ViGretl *mGretl;


};

#endif
