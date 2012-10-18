#include "viplaybackhandler.h"

ViPlaybackHandler::ViPlaybackHandler(ViProcessingChain *chain)
	: ViHandler(chain)
{
	mWaitingForFormat = false;
	mOutput = NULL;
	mCurrentObject = ViAudioObject::createNull();
	QObject::connect(mChain, SIGNAL(streamOutputChanged(ViStreamOutput*)), this, SLOT(changeOutput(ViStreamOutput*)));
	QObject::connect(&mChain->mAudioObjects, SIGNAL(enqueued(ViAudioObjectPointer)), this, SLOT(update(ViAudioObjectPointer)));
}

ViPlaybackHandler::~ViPlaybackHandler()
{
	if(mOutput != NULL)
	{
		QObject::disconnect(mOutput, SIGNAL(finished()), this, SLOT(start()));
		mOutput = NULL;
	}
}

void ViPlaybackHandler::changeOutput(ViStreamOutput *output)
{
	if(mOutput != NULL)
	{
		QObject::disconnect(mOutput, SIGNAL(finished()), this, SLOT(start()));
	}
	mOutput = output;
	QObject::connect(mOutput, SIGNAL(finished()), this, SLOT(start()));
}

void ViPlaybackHandler::update(ViAudioObjectPointer object)
{
	mObjects.enqueue(object);
	start();
}

void ViPlaybackHandler::start()
{
	if(isEnabled())
	{
		if(mWaitingForFormat)
		{
			mWaitingForFormat = false;
			QObject::disconnect(mObjects.first()->correctedBuffer(), SIGNAL(formatChanged(ViAudioFormat)), this, SLOT(start()));
		}
		if(mChain->mStreamOutput->state() != QAudio::ActiveState && mObjects.size() > 0)
		{
			if(!mObjects.first()->correctedBuffer()->format().isValid())
			{
				mCurrentObject = ViAudioObject::createNull();
				mWaitingForFormat = true;
				QObject::connect(mObjects.first()->correctedBuffer(), SIGNAL(formatChanged(ViAudioFormat)), this, SLOT(start()));
			}
			else
			{
				mCurrentObject = mObjects.dequeue();
				mChain->mStreamOutput->setBuffer(mCurrentObject->correctedBuffer());
				mChain->mStreamOutput->start();
			}
		}
	}
}

ViAudioObjectPointer ViPlaybackHandler::currentObject()
{
	return mCurrentObject;
}
