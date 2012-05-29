#ifndef VISAMPLECORRELATOR_H
#define VISAMPLECORRELATOR_H

#include "vicorrelatorstrategy.h"

class ViSampleCorrelator : public ViCorrelatorStrategy
{

	public:
		
		ViSampleCorrelator();
		void initialize(qint32 windowSize = 0);
		void run();
		void finalize();

	private:

		qreal mMaximumDifference;
		qreal mMinimumDifference;
		qreal mAverageDifference;
		qint32 mCounter;
		
};

#endif
