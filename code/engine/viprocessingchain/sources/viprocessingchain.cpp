#include "viprocessingchain.h"

#include "viunderrunhandler.h"
#include "viprojecthandler.h"
#include "visectionhandler.h"

ViProcessingChain::ViProcessingChain()
	: QObject()
{
	mInput = NULL;
	mStreamOutput = NULL;
	mFileOutput = NULL;
	mMultiExecutor.setNotify(true);
	QObject::connect(&mMultiExecutor, SIGNAL(progressed(short)), this, SIGNAL(changed()));

	mUnderrunHandler = new ViUnderrunHandler(this);
	mProjectHandler = new ViProjectHandler(this);
	mSectionHandler = new ViSectionHandler(this);
	mHandlers.append(mUnderrunHandler);
	mHandlers.append(mProjectHandler);
	mHandlers.append(mSectionHandler);
}

ViProcessingChain::~ViProcessingChain()
{
	qDeleteAll(mHandlers);
	mHandlers.clear();

	qDeleteAll(mAudioObjects);
	mAudioObjects.clear();
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
		emit outputChanged();
	}
	else if((fileOutput = dynamic_cast<ViFileOutput*>(transmission)) != NULL)
	{
		mFileOutput = fileOutput;
		emit outputChanged();
	}
}

void ViProcessingChain::setProject(ViProject *project, ViAudioFormat format)
{
	mProjectHandler->create(project, format);
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

bool ViProcessingChain::isSongRunning()
{
	return mSectionHandler->isSongRunning();
}

bool ViProcessingChain::wasSongRunning()
{
	return mSectionHandler->wasSongRunning();
}
