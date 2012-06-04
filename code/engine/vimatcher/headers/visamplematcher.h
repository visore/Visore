#ifndef VISAMPLEMATCHER_H
#define VISAMPLEMATCHER_H

#include "vimatcherstrategy.h"

class ViSampleMatcher : public ViMatcherStrategy
{

	public:
		
		ViSampleMatcher();
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
