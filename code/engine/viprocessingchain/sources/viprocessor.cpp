#include "viprocessor.h"

ViProcessor::ViProcessor(ViProcessor::Type type)
	: QObject(), QRunnable()
{
	setAutoDelete(false); //Ensures that QThreadPool doesn't automatically delete object
	mType = type;
	mIsEnabled = true;
}

ViProcessor::Type ViProcessor::type()
{
	return mType;
}

void ViProcessor::enable(bool enable)
{
	mIsEnabled = enable;
	emit enabled(mIsEnabled);
}

bool ViProcessor::isEnabled()
{
	return mIsEnabled;
}

ViRawProcessor::ViRawProcessor(ViProcessor::Type type)
	: ViProcessor(type)
{
	mData = NULL;
}

void ViRawProcessor::setData(ViChunk<char> *data)
{
	mData = data;
}

ViRealProcessor::ViRealProcessor(ViProcessor::Type type)
	: ViProcessor(type)
{
	mData = NULL;
}

void ViRealProcessor::setData(ViChunk<double> *data)
{
	mData = data;
}
