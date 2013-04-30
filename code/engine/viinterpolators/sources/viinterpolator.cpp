#include <viinterpolator.h>

ViInterpolator::ViInterpolator()
	: ViLibrary()
{
	mLeftData = NULL;
	mRightData = NULL;
}

ViInterpolator::ViInterpolator(const ViInterpolator &other)
	: ViLibrary(other)
{
	mLeftData = new ViSampleChunk(other.mLeftData);
	mRightData = new ViSampleChunk(other.mRightData);
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
