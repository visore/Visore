#ifndef VILINEARPREDICTORS_H
#define VILINEARPREDICTORS_H

#include <vipredictor.h>
#include <vimodelsolver.h>

class ViLinearPredictor : public ViPredictor
{

	public:

		ViLinearPredictor* clone();

	protected:

		qreal predictValue(const qreal *samples, const int &size);
		QList<qreal> predictValues(const qreal *samples, const int &size, const int &length);

	private:

		ViModelSolver mSolver;

};

#endif
