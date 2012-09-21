#include "viprojecthandler.h"
#include "viaudiocodec.h"

#define MINIMUM_SONG_LENGTH 1500

ViProjectHandler::ViProjectHandler(ViProcessingChain *chain)
	: ViHandler(chain)
{
	mProject = NULL;
	QObject::connect(&mChain->mAudioObjects, SIGNAL(finished(ViAudioObject*)), this, SLOT(addAudioObject(ViAudioObject*)));
}

void ViProjectHandler::create(ViProject *project, ViAudioFormat format)
{
	disableAll();
	mChain->mFileOutput->setFormat(format);
	mProject = project;
	mProject->save();
}

void ViProjectHandler::addAudioObject(ViAudioObject *object)
{
	ViAudioBuffer *buffer = object->correctedBuffer();
	qreal songLength = ViAudioPosition::convertPosition(buffer->size(), ViAudioPosition::Samples, ViAudioPosition::Milliseconds, buffer->format());
	if(mChain->wasSongRunning()) //&& songLength > MINIMUM_SONG_LENGTH)
	{
		QObject::connect(mChain->mFileOutput, SIGNAL(finished()), this, SLOT(finishWriting()));
		mChain->mFileOutput->setBuffer(buffer);
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
}
