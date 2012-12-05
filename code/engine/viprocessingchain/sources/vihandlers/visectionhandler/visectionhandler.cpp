#include "visectionhandler.h"
#include "vifrequencyenddetector.h"

#define IDLE_SIZE 5242880 // 5MB
#define IDLE_TIME 1000 // 1 second

ViSectionHandler::ViSectionHandler(ViProcessingChain *chain)
	: ViHandler(chain)
{
	mIdleSize = IDLE_SIZE;
	QObject::connect(&mIdleTimer, SIGNAL(timeout()), this, SLOT(checkSize()));

	mEndDetector = NULL;
	mSpectrumAnalyzer = NULL;
	mIsSongRunning = false;
	mWasSongRunning = false;
	QObject::connect(mChain, SIGNAL(attached(ViProcessor*)), this, SLOT(setDetector(ViProcessor*)));

	mAcceptFinish = false;
	mPlayAutomatically = true;
	mIsPlaying = false;
	QObject::connect(mChain, SIGNAL(inputChanged()), this, SLOT(startInput()));

	QObject::connect(this, SIGNAL(songStarted()), mChain, SIGNAL(songStarted()));
	QObject::connect(this, SIGNAL(songEnded()), mChain, SIGNAL(songEnded()));
	QObject::connect(this, SIGNAL(recordStarted()), mChain, SIGNAL(recordStarted()));
	QObject::connect(this, SIGNAL(recordEnded()), mChain, SIGNAL(recordEnded()));
}

ViSectionHandler::~ViSectionHandler()
{
	mNoSongObjects.clear();
}

bool ViSectionHandler::isSongRunning()
{
	return mIsSongRunning;
}

bool ViSectionHandler::wasSongRunning()
{
	return mWasSongRunning;
}

void ViSectionHandler::setIdleSize(qint64 bytes)
{
	mIdleSize = bytes;
}

void ViSectionHandler::startRecord()
{
	emit recordStarted();
}

void ViSectionHandler::endRecord()
{
	emit recordEnded();
	endInput();
}

void ViSectionHandler::startSong()
{
	endInput();
	startInput(true);
	mIsSongRunning = true;
	mWasSongRunning = false;
	emit songStarted();
}

void ViSectionHandler::endSong()
{
	endInput();
	startInput();
	mAcceptFinish = true;
	while(!executor()->isFinished());
	mIsSongRunning = false;
	mWasSongRunning = true;
	emit songEnded();
}

void ViSectionHandler::startInput(bool isSong)
{
	ViAudioObjectPointer object = ViAudioObject::create();
	object->setType(ViAudioObject::Temp, ViAudioObject::Target);
	if(isSong)
	{
		object->setSong();
		mChain->mAudioObjects.enqueue(object);
	}
	else
	{
		mNoSongObjects.enqueue(object);
		mIdleTimer.start(IDLE_TIME);
	}

	input()->setBuffer(object->inputBuffer());
	executor()->setObject(object);
	executor()->initialize();
	input()->start();
}

void ViSectionHandler::endInput()
{
	input()->stop();
	input()->clear();
	mIdleTimer.stop();
	executor()->finalize();
LOG("-----------------------------*: "+QString::number(mNoSongObjects.size()));
if(mNoSongObjects.size() > 0) LOG("-----------------------------*++++: "+QString::number(mNoSongObjects[0].referenceCount()));
	mNoSongObjects.clear();
LOG("----------------------------- "+QString::number(mNoSongObjects.size()));
	
}

void ViSectionHandler::setInfo(ViSongInfo info)
{
	mChain->audioObject()->setSongInfo(info);
}

void ViSectionHandler::setDetector(ViProcessor *processor)
{
	ViSpectrumAnalyzer *analyzer = dynamic_cast<ViSpectrumAnalyzer*>(processor);
	if(analyzer != NULL)
	{
		mSpectrumAnalyzer = analyzer;
	}
	else
	{
		ViSongDetector *detector = dynamic_cast<ViSongDetector*>(processor);
		if(detector != NULL)
		{
			QObject::connect(detector, SIGNAL(songDetected(ViSongInfo)), this, SLOT(setInfo(ViSongInfo)));
		}
		else
		{
			ViEndDetector *detector = dynamic_cast<ViEndDetector*>(processor);
			if(detector != NULL)
			{
				mEndDetector = detector;
				QObject::connect(mEndDetector, SIGNAL(songStarted(ViAudioPosition)), this, SLOT(startSong()));
				QObject::connect(mEndDetector, SIGNAL(songEnded(ViAudioPosition)), this, SLOT(endSong()));
				QObject::connect(mEndDetector, SIGNAL(recordStarted(ViAudioPosition)), this, SLOT(startRecord()));
				QObject::connect(mEndDetector, SIGNAL(recordEnded(ViAudioPosition)), this, SLOT(endRecord()));
			}
		}
	}
	if(mSpectrumAnalyzer != NULL && mEndDetector != NULL)
	{
		QObject::connect(mSpectrumAnalyzer, SIGNAL(changed(ViRealSpectrum, qint64)), dynamic_cast<ViFrequencyEndDetector*>(mEndDetector), SLOT(addSpectrum(ViRealSpectrum)));
	}
}

void ViSectionHandler::checkSize()
{
	if(mNoSongObjects.current()->inputBuffer()->size() >= mIdleSize)
	{
		endInput();
		startInput();
	}
}

ViAudioInput* ViSectionHandler::input()
{
	return mChain->mInput;
}

ViAudioOutput* ViSectionHandler::output()
{
	return mChain->mStreamOutput;
}

ViExecutor* ViSectionHandler::executor()
{
	return &mChain->mMultiExecutor;
}
