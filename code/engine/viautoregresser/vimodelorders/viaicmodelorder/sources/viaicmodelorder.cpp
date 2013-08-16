#include <viaicmodelorder.h>
#include <qmath.h>

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

ViAicModelOrder* ViAicModelOrder::clone()
{
	return new ViAicModelOrder(*this);
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

#ifdef __cplusplus
extern "C"
{
#endif

ViAicModelOrder* create()
{
	return new ViAicModelOrder();
}

#ifdef __cplusplus
}
#endif
