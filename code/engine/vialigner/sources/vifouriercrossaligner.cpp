#include <vifouriercrossaligner.h>
#include <vifouriertransformer.h>
#include <vilogger.h>

ViFourierCrossAlignerThread::ViFourierCrossAlignerThread()
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

void ViFourierCrossAlignerThread::setRange(qint64 start, qint64 end)
{
	mStart = start;
	mEnd = end;
}

void ViFourierCrossAlignerThread::setParameters(qreal mean1, qreal mean2, qreal denominator)
{
	mMean1 = mean1;
	mMean2 = mean2;
	mDenominator = denominator;
}

void ViFourierCrossAlignerThread::setChunks(ViSampleChunk chunk1, ViSampleChunk chunk2)
{
	mChunk1 = chunk1;
	mChunk2 = chunk2;
}

void ViFourierCrossAlignerThread::setMaximumDelay(qreal delay)
{
	mMaximumDelay = delay;
}

qint64 ViFourierCrossAlignerThread::delay()
{
	QMutexLocker locker(&mMutex);
	return mDelay;
}

qreal ViFourierCrossAlignerThread::correlation()
{
	QMutexLocker locker(&mMutex);
	return mCorrelation;
}

qreal ViFourierCrossAlignerThread::progress()
{
	QMutexLocker locker(&mMutex);
	return mProgress;
}

void ViFourierCrossAlignerThread::run()
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

ViFourierCrossAligner::ViFourierCrossAligner()
	: ViAligner()
{
}

void ViFourierCrossAligner::execute()
{
	QMutexLocker locker(&mMutex);
	mRunningThreads = 0;
	mCurrentDelay = 0;
	mCurrentCorrelation = -1;

	//Pad the chunks to accomodate negative delays
	mChunk1.insert(0, mMaximumDelay);
	mChunk2.append(0, mMaximumDelay);
	int newMaximumDelay = mMaximumDelay * 2;

	// Source: http://dsp.stackexchange.com/questions/736/how-do-i-implement-cross-correlation-to-prove-two-audio-files-are-similar

	qreal theNorm = norm(mChunk1.data(), newMaximumDelay) * norm(mChunk2.data(), newMaximumDelay);

	ViFourierTransformer mTransformer;
	mTransformer.setSize(newMaximumDelay);

	qreal *firstFourierData = new qreal[newMaximumDelay];
	qreal *secondFourierData = new qreal[newMaximumDelay];
	qreal *multiplyData = new qreal[newMaximumDelay];

	mTransformer.forwardTransform(mChunk1.data(), firstFourierData);
	mTransformer.conjugate(firstFourierData);

	mTransformer.forwardTransform(mChunk2.data(), secondFourierData);
	mTransformer.multiply(firstFourierData, secondFourierData, multiplyData);
	mTransformer.inverseTransform(multiplyData, firstFourierData);
	mTransformer.rescale(firstFourierData);
	applyNorm(firstFourierData, newMaximumDelay, theNorm);

	for(int i = 0; i < newMaximumDelay; ++i)
	{
		if(firstFourierData[i] > mCurrentCorrelation)
		{
			mCurrentCorrelation = firstFourierData[i];
			mCurrentDelay = i - mMaximumDelay;
		}
	}

	delete [] firstFourierData;
	delete [] secondFourierData;
	delete [] multiplyData;
	setDelay(mCurrentDelay);
}

qreal ViFourierCrossAligner::norm(qreal array[], qint32 size)
{
	qreal norm = 0;
	for(int i = 0; i < size; ++i)
	{
		norm += (array[i] * array[i]);
	}
	return qSqrt(norm);
}

void ViFourierCrossAligner::applyNorm(qreal array[], qint32 size, qreal norm)
{
	if(norm != 0)
	{
		for(int i = 0; i < size; ++i)
		{
			array[i] /= norm;
		}
	}
}
