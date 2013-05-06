#include <viinterpolator.h>

ViInterpolator::ViInterpolator()
	: ViLibrary()
{
	mLeftData = NULL;
	mRightData = NULL;
	mRatio = 0;
}

ViInterpolator::ViInterpolator(const ViInterpolator &other)
	: ViLibrary(other)
{
	mLeftData = new ViSampleChunk(other.mLeftData);
	mRightData = new ViSampleChunk(other.mRightData);
	mRatio = other.mRatio;
}

ViInterpolator::~ViInterpolator()
{
	clear();
}

void ViInterpolator::clear()
{
	clearLeft();
	clearRight();
}

void ViInterpolator::clearLeft()
{
	if(mLeftData != NULL)
	{
		delete mLeftData;
		mLeftData = NULL;
	}
}

void ViInterpolator::clearRight()
{
	if(mRightData != NULL)
	{
		delete mRightData;
		mRightData = NULL;
	}
}

void ViInterpolator::setData(ViSampleChunk *left, ViSampleChunk *right)
{
	setLeftData(left);
	setRightData(right);
}

void ViInterpolator::setLeftData(ViSampleChunk *data)
{
	clearLeft();
	mLeftData = data;
}

void ViInterpolator::setRightData(ViSampleChunk *data)
{
	clearRight();
	mRightData = data;
}

void ViInterpolator::setData(ViSampleChunk &left, ViSampleChunk &right)
{
	setLeftData(left);
	setRightData(right);
}

void ViInterpolator::setLeftData(ViSampleChunk &data)
{
	clearLeft();
	mLeftData = new ViSampleChunk(data);
}

void ViInterpolator::setRightData(ViSampleChunk &data)
{
	clearRight();
	mRightData = new ViSampleChunk(data);
}

void ViInterpolator::setData(qreal left, qreal right)
{
	setLeftData(left);
	setRightData(right);
}

void ViInterpolator::setLeftData(qreal data)
{
	clearLeft();
	mLeftData = new ViSampleChunk(1);
	(*mLeftData)[0] = data;
}

void ViInterpolator::setRightData(qreal data)
{
	clearRight();
	mRightData = new ViSampleChunk(1);
	(*mRightData)[0] = data;
}

void ViInterpolator::setRatioSamples(const int &samples)
{
	mRatio = 1.0 / (samples + 1);
}

qreal ViInterpolator::ratio(const int &index) const
{
	return mRatio * (index + 1);
}
