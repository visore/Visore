#include "viprocessingchain.h"

#include "viunderrunhandler.h"
#include "viprojecthandler.h"
#include "visectionhandler.h"
#include "viplaybackhandler.h"

ViProcessingChain::ViProcessingChain()
	: QObject()
{
	mInput = NULL;
	mStreamOutput = NULL;
	mFileOutput = NULL;
	mMultiExecutor.setNotify(true);
	QObject::connect(&mMultiExecutor, SIGNAL(progressed(short)), this, SIGNAL(changed()));

	//mUnderrunHandler = new ViUnderrunHandler(this);
	//mProjectHandler = new ViProjectHandler(this);
	mSectionHandler = new ViSectionHandler(this);
	//mPlaybackHandler = new ViPlaybackHandler(this);
	//mHandlers.append(mUnderrunHandler);
	//mHandlers.append(mProjectHandler);
	mHandlers.append(mSectionHandler);
	//mHandlers.append(mPlaybackHandler);
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

void ViProcessingChain::setProject(QString name, QString filePath, ViAudioFormat format, short recordSides)
{
	mInput->setFormat(format);
	mProjectHandler->create(name, filePath, recordSides);
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

ViAudioObjectPointer ViProcessingChain::recordingObject()
{
	return mSectionHandler->currentObject();
}

ViAudioObjectPointer ViProcessingChain::playingObject()
{
	return mPlaybackHandler->currentObject();
}

bool ViProcessingChain::isSongRunning()
{
	return mSectionHandler->isSongRunning();
}

bool ViProcessingChain::wasSongRunning()
{
	return mSectionHandler->wasSongRunning();
}
