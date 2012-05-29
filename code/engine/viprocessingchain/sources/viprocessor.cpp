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
	mInputData = NULL;
	mOutputData = NULL;
}

void ViObserver::setData(const ViSampleChunk *inputData, const ViSampleChunk *outputData)
{
	mInputData = inputData;
	mOutputData = outputData;
}

void ViObserver::setInputData(const ViSampleChunk *data)
{
	mInputData = data;
}

void ViObserver::setOutputData(const ViSampleChunk *data)
{
	mOutputData = data;
}

ViModifier::ViModifier()
	: ViProcessor()
{
	mInputData = NULL;
	mOutputData = NULL;
}

void ViModifier::setData(ViSampleChunk *inputData, ViSampleChunk *outputData)
{
	mInputData = inputData;
	mOutputData = outputData;
}

void ViModifier::setInputData(ViSampleChunk *data)
{
	mInputData = data;
}

void ViModifier::setOutputData(ViSampleChunk *data)
{
	mOutputData = data;
}
