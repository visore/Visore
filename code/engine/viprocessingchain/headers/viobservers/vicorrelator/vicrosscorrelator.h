#ifndef VICROSSCORRELATIONCORRELATOR_H
#define VICROSSCORRELATIONCORRELATOR_H

#include "vicorrelatorstrategy.h"
#include "vifouriertransformer.h"
#include <QThread>

class ViCrossCorrelator : public ViCorrelatorStrategy
{

	public:
		
		ViCrossCorrelator();
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
