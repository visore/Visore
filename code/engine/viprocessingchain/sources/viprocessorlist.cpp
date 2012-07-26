#include "viprocessorlist.h"

ViProcessorList::ViProcessorList()
	: QObject()
{
	mThreadPool.setMaxThreadCount(QThread::idealThreadCount());
}

ViProcessorList::~ViProcessorList()
{
	clear();
}

QList<ViProcessor*> ViProcessorList::processors(int type)
{
	QList<ViProcessor*> result;
	if(type & InputObservers)
	{
		for(int i = 0; i < mInputObservers.size(); ++i)
		{
			result.append(mInputObservers[i]);
		}
	}
	if(type & InputManipulators)
	{
		for(int i = 0; i < mInputModifiers.size(); ++i)
		{
			result.append(mInputModifiers[i]);
		}
	}
	if(type & OutputObservers)
	{
		for(int i = 0; i < mOutputObservers.size(); ++i)
		{
			result.append(mOutputObservers[i]);
		}
	}
	if(type & DualObservers)
	{
		for(int i = 0; i < mDualObservers.size(); ++i)
		{
			result.append(mDualObservers[i]);
		}
	}
	return result;
}

void ViProcessorList::observeInput(const ViSampleChunk *data)
{
	for(int i = 0; i < mInputObservers.size(); ++i)
	{
		mInputObservers[i]->setData(data);
		mThreadPool.start(mInputObservers[i]);
	}
	mThreadPool.waitForDone();
}

void ViProcessorList::manipulateInput(ViSampleChunk *data)
{
	for(int i = 0; i < mInputModifiers.size(); ++i)
	{
		mInputModifiers[i]->setData(data);
		mInputModifiers[i]->run();
	}
}

void ViProcessorList::observeOutput(const ViSampleChunk *data)
{
	for(int i = 0; i < mOutputObservers.size(); ++i)
	{
		mOutputObservers[i]->setData(data);
		mThreadPool.start(mOutputObservers[i]);
	}
	mThreadPool.waitForDone();
}

void ViProcessorList::observeDual(const ViSampleChunk *data, const ViSampleChunk *data2)
{
	for(int i = 0; i < mDualObservers.size(); ++i)
	{
		mDualObservers[i]->setData(data, data2);
		mThreadPool.start(mDualObservers[i]);
	}
	mThreadPool.waitForDone();
}

bool ViProcessorList::add(ViAudio::Mode mode, ViProcessor *processor)
{
	bool result = false;
	ViDualObserver *dualObserver;
	ViObserver *observer;
	ViModifier *modifier;
	if((dualObserver = dynamic_cast<ViDualObserver*>(processor)) != NULL)
	{
		mDualObservers.append(dualObserver);
		result = true;
	}
	else if((observer = dynamic_cast<ViObserver*>(processor)) != NULL)
	{
		if(mode == ViAudio::AudioInput)
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
		if(mode == ViAudio::AudioInput)
		{
			mInputModifiers.append(modifier);
			result = true;
		}
	}
	return result;
}

bool ViProcessorList::remove(ViProcessor *processor)
{
	int index;
	for(index = 0; index < mDualObservers.size(); ++index)
	{
		if(mDualObservers[index] == processor)
		{
			mDualObservers.removeAt(index);
			return true;
		}
	}
	for(index = 0; index < mInputObservers.size(); ++index)
	{
		if(mInputObservers[index] == processor)
		{
			mInputObservers.removeAt(index);
			return true;
		}
	}
	for(index = 0; index < mInputModifiers.size(); ++index)
	{
		if(mInputModifiers[index] == processor)
		{
			mInputModifiers.removeAt(index);
			return true;
		}
	}
	for(index = 0; index < mOutputObservers.size(); ++index)
	{
		if(mOutputObservers[index] == processor)
		{
			mOutputObservers.removeAt(index);
			return true;
		}
	}
	return false;
}

void ViProcessorList::clear()
{
	mInputObservers.clear();
	mInputModifiers.clear();
	mOutputObservers.clear();
	mDualObservers.clear();
}
