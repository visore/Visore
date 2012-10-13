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
	return mResult;
}

void ViCorrelator::setData(const ViSampleChunk *inputData, const ViSampleChunk *outputData)
{
	ViDualObserver::setData(inputData, outputData);
	for(int i = 0; i < mCorrelators.size(); ++i)
	{
		mCorrelators[i]->setData(mData, mData2);
	}
}

void ViCorrelator::setWindowSize(int windowSize)
{
	ViDualObserver::setWindowSize(windowSize);
	for(int i = 0; i < mCorrelators.size(); ++i)
	{
		mCorrelators[i]->setWindowSize(mWindowSize);
	}
}

void ViCorrelator::initialize()
{
	for(int i = 0; i < mCorrelators.size(); ++i)
	{
		mCorrelators[i]->initialize();
	}
}

void ViCorrelator::finalize()
{
	for(int i = 0; i < mCorrelators.size(); ++i)
	{
		mCorrelators[i]->finalize();
	}
}

void ViCorrelator::run()
{
	for(int i = 0; i < mCorrelators.size(); ++i)
	{
		mThreadPool.start(mCorrelators[i]);
	}
	mThreadPool.waitForDone();
}
