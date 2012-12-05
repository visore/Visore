#include "viprocessingchain.h"

#include "viunderrunhandler.h"
#include "visectionhandler.h"
#include "viplaybackhandler.h"
#include "viprojecthandler.h"

ViProcessingChain::ViProcessingChain()
	: QObject()
{
	mInput = NULL;
	mStreamOutput = NULL;
	mFileOutput = NULL;
	mMultiExecutor.setNotify(true);
	QObject::connect(&mMultiExecutor, SIGNAL(progressed(short)), this, SIGNAL(changed()));

	//mUnderrunHandler = new ViUnderrunHandler(this);
	mSectionHandler = new ViSectionHandler(this);
	//mPlaybackHandler = new ViPlaybackHandler(this);
	//mHandlers.append(mUnderrunHandler);
	mHandlers.append(mSectionHandler);
	//mHandlers.append(mPlaybackHandler);

	mProjectHandler = new ViProjectHandler(this);
	mHandlers.append(mProjectHandler);
}

ViProcessingChain::~ViProcessingChain()
{
	qDeleteAll(mHandlers);
	mHandlers.clear();
}

void ViProcessingChain::setWindowSize(int windowSize)
{
	mMultiExecutor.setWindowSize(windowSize);
}

void ViProcessingChain::setTransmission(ViAudioTransmission *transmission)
{
	ViAudioInput *input;
	ViStreamOutput *streamOutput;
	ViFileOutput *fileOutput;
	if((input = dynamic_cast<ViAudioInput*>(transmission)) != NULL)
	{
		mInput = input;
		emit inputChanged();
	}
	else if((streamOutput = dynamic_cast<ViStreamOutput*>(transmission)) != NULL)
	{
		mStreamOutput = streamOutput;
		emit streamOutputChanged(mStreamOutput);
	}
	else if((fileOutput = dynamic_cast<ViFileOutput*>(transmission)) != NULL)
	{
		mFileOutput = fileOutput;
		emit fileOutputChanged(mFileOutput);
	}
}

void ViProcessingChain::startProject(ViProject *project, ViAudio::Type type, bool existingProject)
{
	mProjectHandler->startProject(project, type, existingProject);
}

bool ViProcessingChain::attach(ViAudio::Mode mode, ViProcessor *processor)
{
	mMultiExecutor.attach(mode, processor);
	emit attached(processor);
}

bool ViProcessingChain::detach(ViProcessor *processor)
{
	mMultiExecutor.detach(processor);
}

ViAudioObjectPointer ViProcessingChain::audioObject()
{
	return mAudioObjects.current();
}

ViProcessor* ViProcessingChain::processor(QString type)
{
	return mMultiExecutor.processor(type);
}

bool ViProcessingChain::isSongRunning()
{
	return mSectionHandler->isSongRunning();
}

bool ViProcessingChain::wasSongRunning()
{
	return mSectionHandler->wasSongRunning();
}
