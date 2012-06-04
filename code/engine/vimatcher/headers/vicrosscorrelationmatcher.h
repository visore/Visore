#ifndef VICROSSCORRELATIONMATCHER_H
#define VICROSSCORRELATIONMATCHER_H

#include "vimatcherstrategy.h"
#include "vifouriertransformer.h"
#include <QThread>

class ViCrossCorrelationMatcher : public ViMatcherStrategy
{

	public:
		
		ViCrossCorrelationMatcher();
		void initialize(qint32 windowSize = 0);
		void run();
		void finalize();

	protected:

		void allocateData();
		void deallocateData();
		qreal norm(qreal array[], qint32 size);
		void applyNorm(qreal array[], qint32 size, qreal norm);

	private:

		ViFourierTransformer mTransformer;
		qreal mMaximumDifference;
		qreal mMinimumDifference;
		qreal mAverageDifference;
		qint32 mCounter;

		qreal *mRealData;
		qreal *mFirstFourierData;
		qreal *mSecondFourierData;
		qreal *mMultiplyData;
		qreal *mAutocorrelationData;

};

#endif
