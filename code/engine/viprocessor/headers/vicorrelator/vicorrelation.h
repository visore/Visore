#ifndef VICORRELATION_H
#define VICORRELATION_H

#include <QtGlobal>
#include <QString>

class ViCorrelation
{

	public:

		ViCorrelation(qreal worst = 0, qreal best = 0, qreal average = 0);
		ViCorrelation(const ViCorrelation &other);
		void setWorst(qreal worst);
		void setBest(qreal best);
		void setAverage(qreal average);
		qreal worst();
		qreal best();
		qreal average();
		QString toString();

	private:

		qreal mWorst;
		qreal mBest;
		qreal mAverage;
	
};

#endif
