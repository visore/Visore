#include "vimatcher.h"
#include "visamplematcher.h"
#include "vicrosscorrelationmatcher.h"

#include <iostream>
using namespace std;

ViMatcherThread::ViMatcherThread()
{
	mFirstStream = NULL;
	mSecondStream = NULL;
	mResult = NULL;
	mWindowSize = 0;
	mGlobalPool = QThreadPool::globalInstance();

	mMatchers.append(new ViCrossCorrelationMatcher());
	mMatchers.append(new ViSampleMatcher());
}

ViMatcherThread::~ViMatcherThread()
{
	qDeleteAll(mMatchers);
	mMatchers.clear();
}

void ViMatcherThread::setBuffers(ViAudioBuffer *first, ViAudioBuffer *second)
{
	mFirstStream = first->createReadStream();
	mSecondStream = second->createReadStream();
}

void ViMatcherThread::setResult(ViMatchResult *result)
{
	mResult = result;
}

void ViMatcherThread::setWindowSize(qint32 windowSize)
{
	mWindowSize = windowSize;
}

void ViMatcherThread::run()
{
	emit changed(0);
	qint32 firstSampleSize = mFirstStream->buffer()->format().sampleSize();
	qint32 secondSampleSize = mSecondStream->buffer()->format().sampleSize();
	qint32 firstSize = mWindowSize * (firstSampleSize / 8);
	qint32 secondSize = mWindowSize * (secondSampleSize / 8);
	qint32 counter = 0;
	qint32 totalSize = qMin(mFirstStream->bufferSize() / (firstSampleSize / 8), mSecondStream->bufferSize() / (secondSampleSize / 8));

	char firstRawData[firstSize];
	char secondRawData[secondSize];
	qreal firstRealData[mWindowSize];
	qreal secondRealData[mWindowSize];

	int (*pcmToRealFirstPointer)(char*, qreal*, int);
	int (*pcmToRealSecondPointer)(char*, qreal*, int);

	if(firstSampleSize == 8)
	{
		pcmToRealFirstPointer = &ViPcmConverter<qreal>::pcmToReal8;
	}
	else if(firstSampleSize == 16)
	{
		pcmToRealFirstPointer = &ViPcmConverter<qreal>::pcmToReal16;
	}
	else if(firstSampleSize == 24)
	{
		pcmToRealFirstPointer = &ViPcmConverter<qreal>::pcmToReal24;
	}
	else if(firstSampleSize == 32)
	{
		pcmToRealFirstPointer = &ViPcmConverter<qreal>::pcmToReal32;
	}

	if(secondSampleSize == 8)
	{
		pcmToRealSecondPointer = &ViPcmConverter<qreal>::pcmToReal8;
	}
	else if(secondSampleSize == 16)
	{
		pcmToRealSecondPointer = &ViPcmConverter<qreal>::pcmToReal16;
	}
	else if(secondSampleSize == 24)
	{
		pcmToRealSecondPointer = &ViPcmConverter<qreal>::pcmToReal24;
	}
	else if(secondSampleSize == 32)
	{
		pcmToRealSecondPointer = &ViPcmConverter<qreal>::pcmToReal32;
	}

	initialize();
	while(firstSize > 0 && secondSize > 0)
	{
		secondSize = mSecondStream->read(secondRawData, secondSize);
		firstSize = mFirstStream->read(firstRawData, firstSize);
		if(firstSize > 0 && secondSize > 0)
		{
			firstSampleSize = pcmToRealFirstPointer(firstRawData, firstRealData, firstSize);
			secondSampleSize = pcmToRealSecondPointer(secondRawData, secondRealData, secondSize);
			match(firstRealData, firstSampleSize, secondRealData, secondSampleSize);
			++counter;
			emit changed((counter * mWindowSize) / (totalSize / 100.0));
		}
	}
	finalize();
	emit changed(100);
}

void ViMatcherThread::initialize()
{
	for(int i = 0; i < mMatchers.size(); ++i)
	{
		mMatchers[i]->setResult(mResult);
		mMatchers[i]->initialize(mWindowSize);
	}
}

void ViMatcherThread::match(qreal firstData[], qint32 firstSize, qreal secondData[], qint32 secondSize)
{
	for(int i = 0; i < mMatchers.size(); ++i)
	{
		mMatchers[i]->setData(firstData, firstSize, secondData, secondSize);
		mGlobalPool->start(mMatchers[i]);
	}
	mGlobalPool->waitForDone();
}

void ViMatcherThread::finalize()
{
	for(int i = 0; i < mMatchers.size(); ++i)
	{
		mMatchers[i]->finalize();
	}
}

ViMatcher::ViMatcher()
	: QObject()
{
	QObject::connect(&mThread, SIGNAL(finished()), this, SIGNAL(finished()));
	QObject::connect(&mThread, SIGNAL(changed(qreal)), this, SIGNAL(changed(qreal)));
}

void ViMatcher::match(ViAudioBuffer *first, ViAudioBuffer *second, qint32 windowSize)
{
	mThread.setBuffers(first, second);
	mThread.setResult(&mResult);
	mThread.setWindowSize(windowSize);
	mThread.start();
}

ViMatchResult ViMatcher::result()
{
	return mResult;
}
