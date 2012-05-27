#include "viprocessorlist.h"

ViProcessorList::ViProcessorList()
	: QObject()
{
	mThreadPool.setMaxThreadCount(QThread::idealThreadCount());
}

ViProcessorList::~ViProcessorList()
{
	qDeleteAll(mRawInputObservers);
	mRawInputObservers.clear();
	qDeleteAll(mRealInputObservers);
	mRealInputObservers.clear();
	qDeleteAll(mRawInputModifiers);
	mRawInputModifiers.clear();
	qDeleteAll(mRealInputModifiers);
	mRealInputModifiers.clear();
	qDeleteAll(mRawOutputObservers);
	mRawOutputObservers.clear();
	qDeleteAll(mRealOutputObservers);
	mRealOutputObservers.clear();

	mActiveRawInputObservers.clear();
	mActiveRealInputObservers.clear();
	mActiveRawInputModifiers.clear();
	mActiveRealInputModifiers.clear();
	mActiveRawOutputObservers.clear();
	mActiveRealOutputObservers.clear();
}

void ViProcessorList::updateActiveProcessors()
{
	mActiveRawInputObservers.clear();
	mActiveRealInputObservers.clear();
	mActiveRawInputModifiers.clear();
	mActiveRealInputModifiers.clear();
	mActiveRawOutputObservers.clear();
	mActiveRealOutputObservers.clear();

	for(int i = 0; i < mRawInputObservers.size(); ++i)
	{
		if(mRawInputObservers[i]->isEnabled())
		{
			mActiveRawInputObservers.append(mRawInputObservers[i]);
		}
	}

	for(int i = 0; i < mRealInputObservers.size(); ++i)
	{
		if(mRealInputObservers[i]->isEnabled())
		{
			mActiveRealInputObservers.append(mRealInputObservers[i]);
		}
	}

	for(int i = 0; i < mRawInputModifiers.size(); ++i)
	{
		if(mRawInputModifiers[i]->isEnabled())
		{
			mActiveRawInputModifiers.append(mRawInputModifiers[i]);
		}
	}

	for(int i = 0; i < mRealInputModifiers.size(); ++i)
	{
		if(mRealInputModifiers[i]->isEnabled())
		{
			mActiveRealInputModifiers.append(mRealInputModifiers[i]);
		}
	}

	for(int i = 0; i < mRawOutputObservers.size(); ++i)
	{
		if(mRawOutputObservers[i]->isEnabled())
		{
			mActiveRawOutputObservers.append(mRawOutputObservers[i]);
		}
	}

	for(int i = 0; i < mRealOutputObservers.size(); ++i)
	{
		if(mRealOutputObservers[i]->isEnabled())
		{
			mActiveRealOutputObservers.append(mRealOutputObservers[i]);
		}
	}
}

void ViProcessorList::processRawInputObservers(ViChunk<char> *data)
{
	for(int i = 0; i < mActiveRawInputObservers.size(); ++i)
	{
		mActiveRawInputObservers[i]->setData(data);
		mThreadPool.start(mActiveRawInputObservers[i]);
	}
	mThreadPool.waitForDone();
}

void ViProcessorList::processRealInputObservers(ViChunk<double> *data)
{
	for(int i = 0; i < mActiveRealInputObservers.size(); ++i)
	{
		mActiveRealInputObservers[i]->setData(data);
		mThreadPool.start(mActiveRealInputObservers[i]);
	}
	mThreadPool.waitForDone();
}

void ViProcessorList::processRawInputModifiers(ViChunk<char> *data)
{
	for(int i = 0; i < mActiveRawInputModifiers.size(); ++i)
	{
		mActiveRawInputModifiers[i]->setData(data);
		mActiveRawInputModifiers[i]->run();
	}
}

void ViProcessorList::processRealInputModifiers(ViChunk<double> *data)
{
	for(int i = 0; i < mActiveRealInputModifiers.size(); ++i)
	{
		mActiveRealInputModifiers[i]->setData(data);
		mActiveRealInputModifiers[i]->run();
	}
}

void ViProcessorList::processRawOutputObservers(ViChunk<char> *data)
{
	for(int i = 0; i < mActiveRawOutputObservers.size(); ++i)
	{
		mActiveRawOutputObservers[i]->setData(data);
		mThreadPool.start(mActiveRawOutputObservers[i]);
	}
	mThreadPool.waitForDone();
}

void ViProcessorList::processRealOutputObservers(ViChunk<double> *data)
{
	for(int i = 0; i < mActiveRealOutputObservers.size(); ++i)
	{
		mActiveRealOutputObservers[i]->setData(data);
		mThreadPool.start(mActiveRealOutputObservers[i]);
	}
	mThreadPool.waitForDone();
}

bool ViProcessorList::add(ViAudioConnection::Direction direction, ViProcessor *processor)
{
	bool result = false;
	ViRawProcessor *rawProcessor;
	ViRealProcessor *realProcessor;
	if((rawProcessor = dynamic_cast<ViRawProcessor*>(processor)) != NULL)
	{
		result = addRaw(direction, rawProcessor);
	}
	else if((realProcessor = dynamic_cast<ViRealProcessor*>(processor)) != NULL)
	{
		result = addReal(direction, realProcessor);
	}
	if(result)
	{
		updateActiveProcessors();
		QObject::connect(processor, SIGNAL(enabled(bool)), this, SLOT(updateActiveProcessors()));
	}
	return result;
}

bool ViProcessorList::addRaw(ViAudioConnection::Direction direction, ViRawProcessor *processor)
{
	if(direction == ViAudioConnection::Input)
	{
		if(processor->type() == ViProcessor::Observer)
		{
			mRawInputObservers.append(processor);
			return true;
		}
		else if(processor->type() == ViProcessor::Modifier)
		{
			mRawInputModifiers.append(processor);
			return true;
		}
	}
	else
	{
		if(processor->type() == ViProcessor::Observer)
		{
			mRawOutputObservers.append(processor);
			return true;
		}
	}
	return false;
}

bool ViProcessorList::addReal(ViAudioConnection::Direction direction, ViRealProcessor *processor)
{
	if(direction == ViAudioConnection::Input)
	{
		if(processor->type() == ViProcessor::Observer)
		{
			mRealInputObservers.append(processor);
			return true;
		}
		else if(processor->type() == ViProcessor::Modifier)
		{
			mRealInputModifiers.append(processor);
			return true;
		}
	}
	else
	{
		if(processor->type() == ViProcessor::Observer)
		{
			mRealOutputObservers.append(processor);
			return true;
		}
	}
	return false;
}
