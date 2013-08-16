#include <vimazzonimodelorder.h>
#include <qmath.h>

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

ViMazzoniModelOrder* ViMazzoniModelOrder::clone()
{
	return new ViMazzoniModelOrder(*this);
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

#ifdef __cplusplus
extern "C"
{
#endif

ViMazzoniModelOrder* create()
{
	return new ViMazzoniModelOrder();
}

#ifdef __cplusplus
}
#endif
