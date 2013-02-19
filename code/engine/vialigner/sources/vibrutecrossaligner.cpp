#include <vibrutecrossaligner.h>
#include <qmath.h>
#include <vilogger.h>

ViBruteCrossAlignerThread::ViBruteCrossAlignerThread()
	: QThread()
{
	mStart = 0;
	mEnd = 0;
	mMean1 = 0;
	mMean2 = 0;
	mDenominator = 0;
	mMaximumDelay = 0;
	mDelay = 0;
	mCorrelation = -1;
	mProgress = 0;
}

void ViBruteCrossAlignerThread::setRange(qint64 start, qint64 end)
{
	mStart = start;
	mEnd = end;
}

void ViBruteCrossAlignerThread::setParameters(qreal mean1, qreal mean2, qreal denominator)
{
	mMean1 = mean1;
	mMean2 = mean2;
	mDenominator = denominator;
}

void ViBruteCrossAlignerThread::setChunks(ViSampleChunk chunk1, ViSampleChunk chunk2)
{
	mChunk1 = chunk1;
	mChunk2 = chunk2;
}

void ViBruteCrossAlignerThread::setMaximumDelay(qreal delay)
{
	mMaximumDelay = delay;
}

qint64 ViBruteCrossAlignerThread::delay()
{
	QMutexLocker locker(&mMutex);
	return mDelay;
}

qreal ViBruteCrossAlignerThread::correlation()
{
	QMutexLocker locker(&mMutex);
	return mCorrelation;
}

qreal ViBruteCrossAlignerThread::progress()
{
	QMutexLocker locker(&mMutex);
	return mProgress;
}

void ViBruteCrossAlignerThread::run()
{
	QMutexLocker locker(&mMutex);
	mDelay = 0;
	mCorrelation = -1;
	mProgress = 0;
	locker.unlock();

	//Calculate the correlation series
	int i, j;
	int diff = mStart - 1;
	int diff2 = mEnd - mStart;
	int n = mChunk1.size();
	qreal sCombined, correlation;
	for(int delay = mStart; delay < mEnd; ++delay)
	{
		sCombined = 0;
		for(i = 0; i < n; i++)
		{
			j = i + delay;
			if(j >= 0 && j < n)
			{
				sCombined += (mChunk1[i] - mMean1) * (mChunk2[j] - mMean2);
			}
		}
		correlation = sCombined / mDenominator; //The correlation coefficient at delay
		locker.relock();
		if(correlation > mCorrelation)
		{
			mCorrelation = correlation;
			mDelay = delay;
		}
		mProgress = ((delay - diff) * 100.0) / diff2;
		locker.unlock();
	}
}

ViBruteCrossAligner::ViBruteCrossAligner()
	: ViAligner()
{
}

void ViBruteCrossAligner::execute()
{
	QMutexLocker locker(&mMutex);
	mRunningThreads = 0;
	mCurrentDelay = 0;
	mCurrentCorrelation = -1;

	//Source: http://paulbourke.net/miscellaneous/correlate/

	int n = mChunk1.size();
	int i;

	//Calculate the mean of the two series
	qreal mean1 = 0;
	qreal mean2 = 0;   
	for(i = 0; i < n; i++)
	{
		mean1 += mChunk1[i];
		mean2 += mChunk2[i];
		emit progressed((i * 5.0) / n);
	}
	mean1 /= n;
	mean2 /= n;

	//Calculate the denominator
	qreal s1 = 0;
	qreal s2 = 0;
	for(i = 0; i < n; i++)
	{
		s1 += (mChunk1[i] - mean1) * (mChunk1[i] - mean1);
		s2 += (mChunk2[i] - mean2) * (mChunk2[i] - mean2);
		emit progressed(5 + (i * 5.0) / n);
	}
	qreal denominator = qSqrt(s1 * s2);

	mRunningThreads = QThread::idealThreadCount();
	mTotalThreads = mRunningThreads;
	int maximum = mMaximumDelay * 2;
	int numberPerThread = qFloor(maximum / qreal(mTotalThreads));
	int numberLastThread = maximum - (numberPerThread * (mTotalThreads - 1)) + 1;
	int current;

	for(i = 0; i < mTotalThreads; ++i)
	{
		ViBruteCrossAlignerThread *thread = new ViBruteCrossAlignerThread();
		thread->setParameters(mean1, mean2, denominator);
		thread->setChunks(mChunk1, mChunk2);
		thread->setMaximumDelay(mMaximumDelay);

		current = (i * numberPerThread) - mMaximumDelay;
		if(i != 0 && i == (mTotalThreads - 1)) // Beware if there is only one thread
		{
			thread->setRange(current, current + numberLastThread);
		}
		else
		{
			thread->setRange(current, current + numberPerThread);
		}
		mThreads.append(thread);
		thread->start();
	}

	qreal progress;
	bool continueProcessing = true;
	locker.unlock();
	while(continueProcessing)
	{
		QThread::msleep(500);
		locker.relock();
		progress = 0;
		for(int i = 0; i < mTotalThreads; ++i)
		{
			if(mThreads[i] != NULL)
			{
				if(mThreads[i]->isFinished())
				{
					--mRunningThreads;
					if(mThreads[i]->correlation() > mCurrentCorrelation)
					{
						mCurrentCorrelation = mThreads[i]->correlation();
						mCurrentDelay = mThreads[i]->delay();
					}
					delete mThreads[i];
					mThreads[i] = NULL;
					progress += 100;
					if(mRunningThreads == 0)
					{
						mThreads.clear();
						setDelay(mCurrentDelay);
						progress = 100;
						continueProcessing = false;
						break;
						
					}
				}
				else
				{
					progress += mThreads[i]->progress();
				}
			}
			else
			{
				progress += 100;
			}
		}
		progress = (progress / mTotalThreads) * 0.99;
		locker.unlock();
		emit progressed(progress);
	}
}
