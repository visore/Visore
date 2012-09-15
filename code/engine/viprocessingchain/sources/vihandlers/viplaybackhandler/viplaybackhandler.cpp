#include "viplaybackhandler.h"

ViPlaybackHandler::ViPlaybackHandler(ViProcessingChain *chain)
	: ViHandler(chain)
{
	QObject::connect(mChain, SIGNAL(songStarted()), this, SLOT(handleStart()));
	QObject::connect(mChain, SIGNAL(songEnded()), this, SLOT(check()));
}

void ViPlaybackHandler::handleStart()
{
	if(mIsEnabled)
	{
		mChain->startOutput();
	}
}

void ViPlaybackHandler::handleEnd()
{
	QObject::disconnect(mChain->mStreamOutput, SIGNAL(finished()), this, SLOT(handleEnd()));
	mChain->endOutput();
}

void ViPlaybackHandler::check()
{
	if(mIsEnabled)
	{
		if(mChain->mStreamOutput->state() == QAudio::ActiveState)
		{
			QObject::connect(mChain->mStreamOutput, SIGNAL(finished()), this, SLOT(handleEnd()), Qt::UniqueConnection);
		}
		else
		{
			handleEnd();
		}
	}
	else
	{
		handleEnd();
	}
}
