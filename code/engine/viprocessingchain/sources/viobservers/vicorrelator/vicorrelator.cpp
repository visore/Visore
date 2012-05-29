#include "vicorrelator.h"
#include "visamplecorrelator.h"
#include "vicrosscorrelator.h"

ViCorrelator::ViCorrelator()
	: ViDualObserver()
{
	mCorrelators.append(new ViCrossCorrelator());
	mCorrelators.append(new ViSampleCorrelator());
	for(int i = 0; i < mCorrelators.size(); ++i)
	{
		mCorrelators[i]->setResult(&mResult);
	}
	mThreadPool.setMaxThreadCount(QThread::idealThreadCount());
}

ViCorrelator::~ViCorrelator()
{
	qDeleteAll(mCorrelators);
	mCorrelators.clear();
}

ViCorrelationResult& ViCorrelator::result()
{
	for(int i = 0; i < mCorrelators.size(); ++i)
	{
		mCorrelators[i]->finalize();
	}
	return mResult;
}

void ViCorrelator::setWindowSize(int windowSize)
{
	ViDualObserver::setWindowSize(windowSize);
	for(int i = 0; i < mCorrelators.size(); ++i)
	{
		mCorrelators[i]->initialize(mWindowSize);
	}
}

void ViCorrelator::run()
{
	for(int i = 0; i < mCorrelators.size(); ++i)
	{
		mCorrelators[i]->setData(mData->data(), mData->size(), mData2->data(), mData2->size());
		mThreadPool.start(mCorrelators[i]);
	}
	mThreadPool.waitForDone();
}
