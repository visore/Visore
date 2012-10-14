#include "viprocessor.h"

ViProcessor::ViProcessor()
	: QObject(), QRunnable(), ViId()
{
	setAutoDelete(false); //Ensures that QThreadPool doesn't automatically delete object
	mWindowSize = 0;
	mIsEnabled = true;
	mObject = NULL;
}

void ViProcessor::setObject(ViAudioObject *object)
{
	mObject = object;
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

void ViProcessor::initialize()
{
}

void ViProcessor::finalize()
{
}

void ViProcessor::run()
{
	if(mIsEnabled)
	{
		execute();
	}
}

void ViProcessor::enable()
{
	mIsEnabledMutex.lock();
	mIsEnabled = true;
	mIsEnabledMutex.unlock();
}

void ViProcessor::disable()
{
	mIsEnabledMutex.lock();
	mIsEnabled = false;
	mIsEnabledMutex.unlock();
}

bool ViProcessor::isEnabled()
{
	return mIsEnabled;
}

bool ViProcessor::isDisabled()
{
	return !mIsEnabled;
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

ViDualObserver::ViDualObserver()
	: ViObserver()
{
	mData2 = NULL;
}

void ViDualObserver::setData(const ViSampleChunk *inputData, const ViSampleChunk *outputData)
{
	mData = inputData;
	mData2 = outputData;
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
