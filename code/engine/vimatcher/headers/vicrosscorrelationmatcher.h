#ifndef VICROSSCORRELATIONMATCHER_H
#define VICROSSCORRELATIONMATCHER_H

#include "vimatcherstrategy.h"
#include "vifouriertransformer.h"

class ViCrossCorrelationMatcher : public ViMatcherStrategy
{
	Q_OBJECT

	private slots:

		void calculateNext();
		void summarize();

	public:
		
		ViCrossCorrelationMatcher();
		~ViCrossCorrelationMatcher();
		void match();

	private:

		ViFourierTransformer mTransformer1;
		ViFourierTransformer mTransformer2;
		bool mFirstFourierFinished;

		qreal mMaximumDifference;
		qreal mMinimumDifference;
		qreal mAverageDifference;
		qreal mSampleCounter;
		qint32 mFirstSize;
		qint32 mSecondSize;
		qint32 mFirstSampleSize;
		qint32 mSecondSampleSize;
		char *mFirstRawData;
		char *mSecondRawData;
		float *mFirstRealData;
		float *mSecondRealData;
		float *mFirstFourierData;
		float *mSecondFourierData;
		int (*pcmToRealFirstPointer)(char*, float*, int);
		int (*pcmToRealSecondPointer)(char*, float*, int);
		
};

#endif
