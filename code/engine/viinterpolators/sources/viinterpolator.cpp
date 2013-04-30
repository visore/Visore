#include <viinterpolator.h>

ViInterpolator::ViInterpolator()
	: ViLibrary()
{
	mData = NULL;
}

ViInterpolator::ViInterpolator(const ViInterpolator &other)
	: ViLibrary(other)
{
	mData = new ViSampleChunk(other.mData);
}

ViInterpolator::~ViInterpolator()
{
	clear();
}

void ViInterpolator::clear()
{
	if(mData != NULL)
	{
		delete mData;
		mData = NULL;
	}
}

void ViInterpolator::setData(ViSampleChunk *data)
{
	clear();
	mData = data;
}
