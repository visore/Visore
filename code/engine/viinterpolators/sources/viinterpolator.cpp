#include <viinterpolator.h>

ViInterpolator::ViInterpolator(int leftSamples, int rightSamples)
	: ViLibrary()
{
	mLeftData = NULL;
	mRightData = NULL;
	mRatio = 0;
	mDeleteLeft = false;
	mDeleteRight = false;
	setLeftSamples(leftSamples);
	setRightSamples(rightSamples);
}

ViInterpolator::ViInterpolator(const ViInterpolator &other)
	: ViLibrary(other)
{
	if(other.mDeleteLeft)
	{
		mDeleteLeft = true;
		mLeftData = new ViSampleChunk(other.mLeftData);
	}
	else
	{
		mDeleteLeft = false;
		mLeftData = other.mLeftData;
	}

	if(other.mDeleteRight)
	{
		mDeleteRight = true;
		mRightData = new ViSampleChunk(other.mRightData);
	}
	else
	{
		mDeleteRight = false;
		mRightData = other.mRightData;
	}
	
	mRatio = other.mRatio;

	mLeftSamples = other.mLeftSamples;
	mRightSamples = other.mRightSamples;
}

ViInterpolator::~ViInterpolator()
{
	clear();
}

int ViInterpolator::leftSamples()
{
	return mLeftSamples;
}

int ViInterpolator::rightSamples()
{
	return mRightSamples;
}

void ViInterpolator::setLeftSamples(int samples)
{
	mLeftSamples = samples;
}

void ViInterpolator::setRightSamples(int samples)
{
	mRightSamples = samples;
}

void ViInterpolator::clear()
{
	clearLeft();
	clearRight();
}

void ViInterpolator::clearLeft()
{
	if(mDeleteLeft && mLeftData != NULL)
	{
		delete mLeftData;
		mLeftData = NULL;
		mDeleteLeft = false;
	}
}

void ViInterpolator::clearRight()
{
	if(mDeleteRight && mRightData != NULL)
	{
		delete mRightData;
		mRightData = NULL;
		mDeleteRight = false;
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
	mDeleteLeft = true;
}

void ViInterpolator::setRightData(qreal data)
{
	clearRight();
	mRightData = new ViSampleChunk(1);
	(*mRightData)[0] = data;
	mDeleteRight = true;
}

void ViInterpolator::setRatioSamples(const int &samples)
{
	mRatio = 1.0 / (samples + 1);
}

qreal ViInterpolator::ratio(const int &index) const
{
	return mRatio * (index + 1);
}
