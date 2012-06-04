#include "vimatcher.h"
#include "visamplematcher.h"
#include "vicrosscorrelationmatcher.h"

#include <iostream>
using namespace std;

ViMatcherThread::ViMatcherThread(ViMatcherStrategy *strategy)
{
	mStrategy = strategy;
}

void ViMatcherThread::run()
{
	mStrategy->match();
}

ViMatcher::ViMatcher()
	: QObject()
{
	finishCounter = 0;
	mStrategies.append(new ViSampleMatcher());
	mStrategies.append(new ViCrossCorrelationMatcher());
	for(int i = 0; i < mStrategies.size(); ++i)
	{
		mStrategies[i]->setResult(&mResult);
		mThreads.append(new ViMatcherThread(mStrategies[i]));
		QObject::connect(mThreads[i], SIGNAL(finished()), this, SLOT(checkFinished()));
	}
}

ViMatcher::~ViMatcher()
{
	qDeleteAll(mThreads.begin(), mThreads.end());
	mThreads.clear();
	qDeleteAll(mStrategies.begin(), mStrategies.end());
	mStrategies.clear();
}

void ViMatcher::match(ViAudioBuffer *first, ViAudioBuffer *second)
{
	for(int i = 0; i < mThreads.size(); ++i)
	{
		mStrategies[i]->setBuffers(first, second);
		mThreads[i]->start();
	}
}

ViMatchResult ViMatcher::result()
{
	return mResult;
}

void ViMatcher::checkFinished()
{
	++finishCounter;
	if(finishCounter == mThreads.size())
	{
		finishCounter = 0;
		emit finished();
cout<<mResult.toString().toAscii().data()<<endl;
	}
}
