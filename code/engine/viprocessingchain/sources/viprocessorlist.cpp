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
	clear();
}

QList<ViProcessor*> ViProcessorList::all()
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

void ViProcessorList::observeInput(const ViSampleChunk *input, const ViSampleChunk *output)
{
	for(int i = 0; i < mInputObservers.size(); ++i)
	{
		mInputObservers[i]->setData(input, output);
		mThreadPool.start(mInputObservers[i]);
	}
	mThreadPool.waitForDone();
}

void ViProcessorList::manipulateInput(ViSampleChunk *input, ViSampleChunk *output)
{
	for(int i = 0; i < mInputModifiers.size(); ++i)
	{
		mInputModifiers[i]->setData(input, output);
		mInputModifiers[i]->run();
	}
}

void ViProcessorList::observeOutput(const ViSampleChunk *input, const ViSampleChunk *output)
{
	for(int i = 0; i < mOutputObservers.size(); ++i)
	{
		mOutputObservers[i]->setData(input, output);
		mThreadPool.start(mOutputObservers[i]);
	}
	mThreadPool.waitForDone();
}

void ViProcessorList::observeInput(const ViSampleChunk *data)
{
	for(int i = 0; i < mInputObservers.size(); ++i)
	{
		mInputObservers[i]->setInputData(data);
		mThreadPool.start(mInputObservers[i]);
	}
	mThreadPool.waitForDone();
}

void ViProcessorList::manipulateInput(ViSampleChunk *data)
{
	for(int i = 0; i < mInputModifiers.size(); ++i)
	{
		mInputModifiers[i]->setInputData(data);
		mInputModifiers[i]->run();
	}
}

void ViProcessorList::observeOutput(const ViSampleChunk *data)
{
	for(int i = 0; i < mOutputObservers.size(); ++i)
	{
		mOutputObservers[i]->setInputData(data);
		mThreadPool.start(mOutputObservers[i]);
	}
	mThreadPool.waitForDone();
}

bool ViProcessorList::add(ViAudio::Mode mode, ViProcessor *processor)
{
	bool result = false;
	ViObserver *observer;
	ViModifier *modifier;
	if((observer = dynamic_cast<ViObserver*>(processor)) != NULL)
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
}
