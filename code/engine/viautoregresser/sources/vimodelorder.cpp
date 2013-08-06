#include <vimodelorder.h>
#include <qmath.h>

ViModelOrder::ViModelOrder()
{
	mSampleCount = 0;
	mParameterCount = 0;
}

ViModelOrder::ViModelOrder(const ViModelOrder &other)
{
	mSampleCount = other.mSampleCount;
	mParameterCount = other.mParameterCount;
}

ViModelOrder::~ViModelOrder()
{
}

void ViModelOrder::clear()
{
	mSampleCount = 0;
	mParameterCount = 0;
}

void ViModelOrder::setParameterCount(const int &parameters)
{
	mParameterCount = parameters;
}

void ViModelOrder::setSampleCount(const int &count)
{
	mSampleCount = count;
}

/********************************************************************
	MAZZONI
********************************************************************/

ViMazzoniModelOrder::ViMazzoniModelOrder()
	: ViModelOrder()
{
	mBadSampleCount = 0;
	mFirstBadSample = 0;
}

ViMazzoniModelOrder::ViMazzoniModelOrder(const ViMazzoniModelOrder &other)
	: ViModelOrder(other)
{
	mBadSampleCount = other.mBadSampleCount;
	mFirstBadSample = other.mFirstBadSample;
}

ViMazzoniModelOrder::~ViMazzoniModelOrder()
{
}

void ViMazzoniModelOrder::clear()
{
	ViModelOrder::clear();
	mBadSampleCount = 0;
	mFirstBadSample = 0;
}

void ViMazzoniModelOrder::setBadSampleCount(const int &count)
{
	mBadSampleCount = count;
}

void ViMazzoniModelOrder::setFirstBadSample(const int &index)
{
	mFirstBadSample = index;
}

int ViMazzoniModelOrder::order() const
{
	return order(mSampleCount, mBadSampleCount, mFirstBadSample);
}

int ViMazzoniModelOrder::order(const int &sampleCount, const int &badSampleCount, const int &firstBadSample)
{
	return qMin(qMin(badSampleCount * 3, 50), qMax(firstBadSample - 1, sampleCount - (firstBadSample + badSampleCount) - 1));
}

/********************************************************************
	AKAIKE'S INFORMATION CRITERIA (AIC)
********************************************************************/

ViAicModelOrder::ViAicModelOrder()
	: ViModelOrder()
{
	mRss = 0;
}

ViAicModelOrder::ViAicModelOrder(const ViAicModelOrder &other)
	: ViModelOrder(other)
{
	mRss = other.mRss;
	mPredictedValues = other.mPredictedValues;
	mRealValues = other.mRealValues;
}

ViAicModelOrder::~ViAicModelOrder()
{
}

void ViAicModelOrder::clear()
{
	ViModelOrder::clear();
	mRss = 0;
	mPredictedValues.clear();
	mRealValues.clear();
}

void ViAicModelOrder::setRss(const qreal &rss)
{
	mRss = rss;
}

void ViAicModelOrder::addRss(const qreal &predictedValue, const qreal &realValue)
{
	mPredictedValues.enqueue(predictedValue);
	mRealValues.enqueue(realValue);

	mRss = 0;
	for(int i = 0; i < mPredictedValues.size(); ++i)
	{
		mRss += qPow(mRealValues[i] - mPredictedValues[i], 2);
	}
}

int ViAicModelOrder::order() const
{
	return order(mSampleCount, mParameterCount, mRss);
}

int ViAicModelOrder::order(const int &sampleCount, const int &parameterCount, const qreal &rss)
{
	return (sampleCount * qLn(rss / sampleCount)) + (2 * parameterCount);
}

/********************************************************************
	AKAIKE'S INFORMATION CRITERIA WITH CORRECTION (AICc)
********************************************************************/

ViAiccModelOrder::ViAiccModelOrder()
	: ViAicModelOrder()
{
}

ViAiccModelOrder::ViAiccModelOrder(const ViAiccModelOrder &other)
	: ViAicModelOrder(other)
{
}

ViAiccModelOrder::~ViAiccModelOrder()
{
}

int ViAiccModelOrder::order(const int &sampleCount, const int &parameterCount, const qreal &rss)
{
	return ViAicModelOrder::order(sampleCount, parameterCount, rss) + ((2 * parameterCount * (parameterCount + 1)) / (sampleCount - parameterCount - 1));
}
