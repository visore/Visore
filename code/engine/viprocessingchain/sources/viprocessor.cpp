#include "viprocessor.h"

ViProcessor::ViProcessor()
	: QObject(), QRunnable()
{
}

ViRawProcessor::ViRawProcessor()
	: ViProcessor()
{
	mData = NULL;
}

void ViRawProcessor::setData(ViChunk<char> *data)
{
	mData = data;
}

ViRealProcessor::ViRealProcessor()
	: ViProcessor()
{
	mData = NULL;
}

void ViRealProcessor::setData(ViChunk<double> *data)
{
	mData = data;
}
