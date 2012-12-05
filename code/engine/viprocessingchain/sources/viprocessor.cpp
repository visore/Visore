#include "viprocessor.h"

ViProcessor::ViProcessor(QString name)
	: QObject(), QRunnable(), ViId()
{
	mName = name;
	setAutoDelete(false); //Ensures that QThreadPool doesn't automatically delete object
	mWindowSize = 0;
	mIsEnabled = true;
	mObject = NULL;
}

void ViProcessor::setObject(ViAudioObjectPointer object)
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

bool ViProcessor::importParameters(const ViElement &element)
{
	if(element.name() == name())
	{
		return true;
	}
	return false;
}

void ViProcessor::exportParameters(ViElement &element)
{
	element.setName(name(true));
}

bool ViProcessor::importResults(const ViElement &element)
{
	if(element.name() == name())
	{
		return true;
	}
	return false;
}

void ViProcessor::exportResults(ViElement &element)
{
	element.setName(name(true));
}

void ViProcessor::enable()
{
	mIsEnabledMutex.lock();
	mIsEnabled = true;
	blockSignals(false);
	mIsEnabledMutex.unlock();
}

void ViProcessor::disable()
{
	mIsEnabledMutex.lock();
	mIsEnabled = false;
	blockSignals(true);
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

QString ViProcessor::name(bool simple)
{
	if(simple && mName.startsWith("Vi"))
	{
		return mName.mid(2);
	}
	return mName;
}

ViObserver::ViObserver(QString name)
	: ViProcessor(name)
{
	mData = NULL;
}

void ViObserver::setData(const ViSampleChunk *data)
{
	mData = data;
}

ViDualObserver::ViDualObserver(QString name)
	: ViObserver(name)
{
	mData2 = NULL;
}

void ViDualObserver::setData(const ViSampleChunk *inputData, const ViSampleChunk *outputData)
{
	mData = inputData;
	mData2 = outputData;
}

ViModifier::ViModifier(QString name)
	: ViProcessor(name)
{
	mData = NULL;
}

void ViModifier::setData(ViSampleChunk *data)
{
	mData = data;
}
