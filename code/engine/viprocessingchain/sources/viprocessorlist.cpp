#include "viprocessorlist.h"

ViProcessorList::ViProcessorList()
	: QObject()
{
	mThreadPool.setMaxThreadCount(QThread::idealThreadCount());
}

ViProcessorList::~ViProcessorList()
{
	qDeleteAll(mInputObservers);
	qDeleteAll(mInputModifiers);
	qDeleteAll(mOutputObservers);
	removeAll();
}

QList<ViProcessor*> ViProcessorList::processors()
{
	QList<ViProcessor*> result;
	for(int i = 0; i < mInputObservers.size(); ++i)
	{
		result.append(mInputObservers[i]);
	}
	for(int i = 0; i < mInputModifiers.size(); ++i)
	{
		result.append(mInputModifiers[i]);
	}
	for(int i = 0; i < mOutputObservers.size(); ++i)
	{
		result.append(mOutputObservers[i]);
	}
	return result;
}

void ViProcessorList::updateActiveProcessors()
{
	mActiveInputObservers.clear();
	mActiveInputModifiers.clear();
	mActiveOutputObservers.clear();

	for(int i = 0; i < mInputObservers.size(); ++i)
	{
		if(mInputObservers[i]->isEnabled())
		{
			mActiveInputObservers.append(mInputObservers[i]);
		}
	}

	for(int i = 0; i < mInputModifiers.size(); ++i)
	{
		if(mInputModifiers[i]->isEnabled())
		{
			mActiveInputModifiers.append(mInputModifiers[i]);
		}
	}

	for(int i = 0; i < mOutputObservers.size(); ++i)
	{
		if(mOutputObservers[i]->isEnabled())
		{
			mActiveOutputObservers.append(mOutputObservers[i]);
		}
	}
}

void ViProcessorList::observeInput(const ViSampleChunk *data)
{
	for(int i = 0; i < mActiveInputObservers.size(); ++i)
	{
		mActiveInputObservers[i]->setData(data);
		mThreadPool.start(mActiveInputObservers[i]);
	}
	mThreadPool.waitForDone();
}

void ViProcessorList::manipulateInput(ViSampleChunk *data)
{
	for(int i = 0; i < mActiveInputModifiers.size(); ++i)
	{
		mActiveInputModifiers[i]->setData(data);
		mActiveInputModifiers[i]->run();
	}
}

void ViProcessorList::observeOutput(const ViSampleChunk *data)
{
	for(int i = 0; i < mActiveOutputObservers.size(); ++i)
	{
		mActiveOutputObservers[i]->setData(data);
		mThreadPool.start(mActiveOutputObservers[i]);
	}
	mThreadPool.waitForDone();
}

bool ViProcessorList::add(ViAudioConnection::Direction direction, ViProcessor *processor)
{
	bool result = false;
	ViObserver *observer;
	ViModifier *modifier;
	if((observer = dynamic_cast<ViObserver*>(processor)) != NULL)
	{
		if(direction == ViAudioConnection::Input)
		{
			mInputObservers.append(observer);
			result = true;
		}
		else
		{
			mOutputObservers.append(observer);
			result = true;
		}
	}
	else if((modifier = dynamic_cast<ViModifier*>(processor)) != NULL)
	{
		if(direction == ViAudioConnection::Input)
		{
			mInputModifiers.append(modifier);
			result = true;
		}
	}
	if(result)
	{
		updateActiveProcessors();
		QObject::connect(processor, SIGNAL(enabled(bool)), this, SLOT(updateActiveProcessors()));
	}
	return result;
}

void ViProcessorList::removeAll()
{
	mInputObservers.clear();
	mInputModifiers.clear();
	mOutputObservers.clear();
	mActiveInputObservers.clear();
	mActiveInputModifiers.clear();
	mActiveOutputObservers.clear();
}
