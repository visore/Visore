#include "viprojecthandler.h"
#include "visongdetector.h"
#include "vitypechecker.h"

ViProjectHandler::ViProjectHandler(ViProcessingChain *chain)
	: ViHandler(chain)
{
	mProject = NULL;
	QObject::connect(&mChain->mAudioObjects, SIGNAL(finished(ViAudioObjectPointer)), this, SLOT(updateProject(ViAudioObjectPointer)));

	QObject::connect(mChain, SIGNAL(songStarted()), this, SLOT(startSong()));
	QObject::connect(mChain, SIGNAL(songEnded()), this, SLOT(endSong()));
	QObject::connect(mChain, SIGNAL(recordStarted()), this, SLOT(startRecord()));
	QObject::connect(mChain, SIGNAL(recordEnded()), this, SLOT(endRecord()));
}

void ViProjectHandler::updateProject(ViAudioObjectPointer object)
{
	if(mProject != NULL)
	{
		mProject->serialize(object, mProjectType);
	}
	mChain->mAudioObjects.clear();
}

void ViProjectHandler::startProject(ViProject *project, ViAudio::Type type, bool existingProject)
{
	mProject = project;
	mProjectType = type;
	if(existingProject)
	{
		QObject::connect(mProject, SIGNAL(finished()), this, SLOT(begin()));
		emit mChain->progressStarted();
		emit mChain->statusChanged("Processing project data");
		mProject->load();
	}
	else
	{
		emit mChain->progressStarted();
		mProject->save();
		begin();
	}
	mChain->mInput->start();
}

void ViProjectHandler::begin()
{
	emit mChain->statusChanged("Waiting for record to start");
}

void ViProjectHandler::startSong()
{
	emit mChain->statusChanged("Processing track");
}

void ViProjectHandler::endSong()
{
	emit mChain->statusChanged("Waiting for track to start");
}

void ViProjectHandler::startRecord()
{
	emit mChain->statusChanged("Waiting for track to start");
}

void ViProjectHandler::endRecord()
{
	if(mProject->currentSide() < mProject->sides())
	{
		emit mChain->statusChanged("Please turn over the record");
		mProject->nextSide();
	}
	else
	{
		mChain->mInput->stop();
		mChain->mInput->clear();
		mChain->mAudioObjects.clear();
		delete mProject;
		mProject = NULL;
		emit mChain->progressFinished();
	}
}
