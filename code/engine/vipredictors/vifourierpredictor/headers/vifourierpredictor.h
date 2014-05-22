#ifndef VIFOURIERPREDICTOR_H
#define VIFOURIERPREDICTOR_H

#include <vipredictor.h>

class ViFourierPredictor : public ViModelPredictor
{

	public:

		ViFourierPredictor(const Estimation &estimation = Fixed);
		ViFourierPredictor(const ViFourierPredictor &other);
		~ViFourierPredictor();

		void setParameter(const int &number, const qreal &value);

	protected:

		bool estimateModel(const int &degree, ViVector &coefficients, const qreal *samples, const int &size);
		void predictModel(const int &degree, const ViVector &coefficients, qreal *prediction, const int &size, const int &start);

};

#endif
