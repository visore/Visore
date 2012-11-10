#ifndef VISAMPLECORRELATOR_H
#define VISAMPLECORRELATOR_H

#include "vicorrelator.h"

class ViSampleCorrelator : public ViCorrelator
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
