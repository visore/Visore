#ifndef VICROSSCORRELATIONMATCHER_H
#define VICROSSCORRELATIONMATCHER_H

#include "vimatcherstrategy.h"
#include "vifouriertransformer.h"

class ViCrossCorrelationMatcher : public ViMatcherStrategy
{

	private slots:

		void calculateNext();

	public:
		
		ViCrossCorrelationMatcher();
		~ViCrossCorrelationMatcher();
		void match();

	private:

		ViFourierTransformer mTransformer;
		qreal mMaximumDifference;
		qreal mMinimumDifference;
		qreal mAverageDifference;
		qreal mSampleCounter;
		qint32 mFirstSize;
		qint32 mSecondSize;
		char *mFirstRawData;
		char *mSecondRawData;
		float *mFirstRealData;
		float *mSecondRealData;
		int (*pcmToRealFirstPointer)(char*, float*, int);
		int (*pcmToRealSecondPointer)(char*, float*, int);
		
};

#endif
