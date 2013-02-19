#ifndef VIFOURIERCROSSALIGNER_H
#define VIFOURIERCROSSALIGNER_H

#include <vialigner.h>

class ViFourierCrossAlignerThread : public QThread
{

	Q_OBJECT

	public:

		ViFourierCrossAlignerThread();
		void setRange(qint64 start, qint64 end);
		void setParameters(qreal mean1, qreal mean2, qreal denominator);
		void setChunks(ViSampleChunk chunk1, ViSampleChunk chunk2);
		void setMaximumDelay(qreal delay);
		qint64 delay();
		qreal correlation();
		qreal progress();
		void run();

	private:

		QMutex mMutex;

		qint64 mStart;
		qint64 mEnd;
		qreal mMean1;
		qreal mMean2;
		qreal mDenominator;

		ViSampleChunk mChunk1;
		ViSampleChunk mChunk2;
		qreal mMaximumDelay;
		qint64 mDelay;
		qreal mCorrelation;
		qreal mProgress;

};

class ViFourierCrossAligner : public ViAligner
{

	Q_OBJECT

	public:
		
		ViFourierCrossAligner();

	protected:

		void execute();
		qreal norm(qreal array[], qint32 size);
		void applyNorm(qreal array[], qint32 size, qreal norm);

	private:

		QMutex mMutex;

		QList<ViFourierCrossAlignerThread*> mThreads;
		int mRunningThreads;
		int mTotalThreads;
		int mCurrentDelay;
		qreal mCurrentCorrelation;
		
};

#endif
