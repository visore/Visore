#ifndef VISAMPLECORRELATOR_H
#define VISAMPLECORRELATOR_H

#include "vicorrelatorstrategy.h"

class ViSampleCorrelator : public ViCorrelatorStrategy
{

	public:
		
		ViSampleCorrelator();
		void initialize();
		void finalize();
		void execute();

	private:

		qreal mMaximumDifference;
		qreal mMinimumDifference;
		qreal mAverageDifference;
		qint32 mCounter;
		
};

#endif
