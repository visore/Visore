#include "viprocessor.h"

ViProcessor::ViProcessor()
	: QObject(), QRunnable(), ViId()
{
	setAutoDelete(false); //Ensures that QThreadPool doesn't automatically delete object
	mWindowSize = 0;
}

void ViProcessor::setWindowSize(int windowSize)
{
	mWindowSize = windowSize;
}

int ViProcessor::windowSize()
{
	return mWindowSize;
}

void ViProcessor::setFormat(ViAudioFormat format)
{
	mFormat = format;
}

ViAudioFormat ViProcessor::format()
{
	return mFormat;
}

ViObserver::ViObserver()
	: ViProcessor()
{
	mData = NULL;
}

void ViObserver::setData(const ViSampleChunk *data)
{
	mData = data;
}

ViModifier::ViModifier()
	: ViProcessor()
{
	mData = NULL;
}

void ViModifier::setData(ViSampleChunk *data)
{
	mData = data;
}

ViDualObserver::ViDualObserver()
	: ViObserver()
{
	mData2 = NULL;
}

void ViDualObserver::setData(const ViSampleChunk *data, const ViSampleChunk *data2)
{
	mData = data;
	mData2 = data2;
}

