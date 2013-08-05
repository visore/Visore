#include <vimodelorder.h>
/*
ViModelOrder::ViModelOrder()
{
}

ViModelOrder::ViModelOrder(const ViModelOrder &other)
{
}

ViModelOrder::~ViModelOrder()
{
}

ViMazzoniModelOrder::ViMazzoniModelOrder()
	: ViModelOrder()
{
	mSampleCount = 0;
	mBadSampleCount = 0;
	mFirstBadSample = 0;
}

ViMazzoniModelOrder::ViMazzoniModelOrder(const ViMazzoniModelOrder &other)
	: ViModelOrder(other)
{
	mSampleCount = other.mSampleCount;
	mBadSampleCount = other.mBadSampleCount;
	mFirstBadSample = other.mFirstBadSample;
}

void ViMazzoniModelOrder::setSampleCount(const int &count)
{
	mSampleCount = count;
}

void ViMazzoniModelOrder::setBadSampleCount(const int &count)
{
	mBadSampleCount = count;
}

void ViMazzoniModelOrder::setFirstBadSample(const int &index)
{
	mFirstBadSample = &index;
}

int ViMazzoniModelOrder::order()
{
	return order(mSampleCount, mBadSampleCount, mFirstBadSample);
}

int ViMazzoniModelOrder::order(const int &sampleCount, const int &badSampleCount, const int &firstBadSample)
{
	return qMin(qMin(badSampleCount * 3, 50), qMax(firstBadSample - 1, sampleCount - (firstBadSample + badSampleCount) - 1));
}

*/
