#include "viprojecthandler.h"
#include "viaudiocodec.h"

#define MINIMUM_SONG_LENGTH 1500
/*
ViProjectHandler::ViProjectHandler(ViProcessingChain *chain)
	: ViHandler(chain)
{
	mProject = NULL;
}

void ViProjectHandler::create(ViProject *project, ViAudioFormat format)
{
	disableAll();
	mChain->mFileOutput->setFormat(format);
	mProject = project;
	mProject->save();
	QObject::connect(mChain, SIGNAL(finishedProcessing()), this, SLOT(finishOff()));
}

void ViProjectHandler::finishOff()
{
	QObject::disconnect(mChain, SIGNAL(finishedProcessing()), this, SLOT(finishOff()));
	qreal songLength = ViAudioPosition::convertPosition(mChain->mOutputBuffer->size(), ViAudioPosition::Samples, ViAudioPosition::Milliseconds, mChain->mOutputBuffer->format());
	if(mChain->wasSongRunning()) //&& songLength > MINIMUM_SONG_LENGTH)
	{
		QObject::connect(mChain->mFileOutput, SIGNAL(finished()), this, SLOT(finishWriting()));
		mChain->mFileOutput->setBuffer(mChain->mOutputBuffer);
		mChain->mFileOutput->setFile(mProject->originalPath(), mProject->nextOriginalSongNumber(), mChain->mFileOutput->format().codec()->extensions()[0]);
		mChain->mFileOutput->start();
	}
}

void ViProjectHandler::finishWriting()
{
	ViFileSongInfo info = ViFileSongInfo(mChain->mFileOutput->songInfo());
	info.setOriginalFilePath(mChain->mFileOutput->filePath());
	mProject->addSong(info);
	mProject->save();
	QObject::disconnect(mChain->mFileOutput, SIGNAL(finished()), this, SLOT(finishWriting()));
	mChain->nextBuffer(ViAudio::AudioOutput);
	//QObject::connect(mChain->mStreamOutput, SIGNAL(finished()), this, SLOT(finishPlaying()));
	//if(mChain->mStreamOutput->state() != QAudio::ActiveState)
	//{
	//	finishPlaying();
	//}
}

void ViProjectHandler::finishPlaying()
{
	QObject::disconnect(mChain->mStreamOutput, SIGNAL(finished()), this, SLOT(finishPlaying()));
	//mChain->endOutput();
	//mChain->startOutput();
	enableAll();
}*/
