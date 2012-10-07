#include "visectionhandler.h"
#include "vifrequencyenddetector.h"

ViSectionHandler::ViSectionHandler(ViProcessingChain *chain)
	: ViHandler(chain)
{
	mEndDetector = NULL;
	mSpectrumAnalyzer = NULL;
	mCurrentSongObject = NULL;
	mIsSongRunning = false;
	mWasSongRunning = false;
	QObject::connect(mChain, SIGNAL(attached(ViProcessor*)), this, SLOT(setDetector(ViProcessor*)));

	mAcceptFinish = false;
	mPlayAutomatically = true;
	mIsPlaying = false;
	QObject::connect(mChain, SIGNAL(inputChanged()), this, SLOT(startInput()));
}

ViSectionHandler::~ViSectionHandler()
{
	qDeleteAll(mNoSongObjects);
	mNoSongObjects.clear();
}

bool ViSectionHandler::isSongRunning()
{
	QMutexLocker locker(&mMutex);
	return mIsSongRunning;
}

bool ViSectionHandler::wasSongRunning()
{
	QMutexLocker locker(&mMutex);
	return mWasSongRunning;
}

void ViSectionHandler::startRecord()
{
	QMutexLocker locker(&mMutex);
	//mChain->startInput();
	emit mChain->recordStarted();
}

void ViSectionHandler::endRecord()
{
	QMutexLocker locker(&mMutex);
	emit mChain->recordEnded();
}

void ViSectionHandler::startSong()
{
	QMutexLocker locker(&mMutex);
	endInput();
	startInput(true);
	//startOutput();

	mIsSongRunning = true;
	mWasSongRunning = false;
	/*mChain->endInput();
	mChain->startInput();
	if(!mWasSongRunning)
	{
		//mChain->startOutput();
	}*/
	emit mChain->songStarted();
}

void ViSectionHandler::endSong()
{
	QMutexLocker locker(&mMutex);
	endInput();
	startInput();
	mAcceptFinish = true;
	while(!executor()->isFinished())
	{
		finish();
	}



	mIsSongRunning = false;
	mWasSongRunning = true;
	/*mChain->endInput();
	mChain->startInput();*/
	emit mChain->songEnded();
}

void ViSectionHandler::startInput(bool isSong)
{
	//QMutexLocker locker(&mMutex);
	ViAudioBuffer *inputBuffer = allocateBuffer();
	ViAudioBuffer *outputBuffer = allocateBuffer();
	ViAudioObject *object = new ViAudioObject(inputBuffer, outputBuffer);
	if(isSong)
	{
		mChain->mAudioObjects.enqueue(object);
		mCurrentSongObject = object;
	}
	else
	{
		mNoSongObjects.enqueue(object);
	}

	input()->setBuffer(inputBuffer);
	executor()->setBuffer(ViAudio::AudioInput, inputBuffer);
	executor()->setBuffer(ViAudio::AudioOutput, outputBuffer);
	executor()->initialize();
}

void ViSectionHandler::endInput()
{
	executor()->finalize();
	if(mCurrentSongObject != NULL)
	{
		mCurrentSongObject->setFinished();
		mCurrentSongObject = NULL;
	}

	qDeleteAll(mNoSongObjects);
	mNoSongObjects.clear();
	/*if(mInputBuffer != NULL)
	{
		mInputBuffers.enqueue(mInputBuffer);
	}*/
	/*if((!mPlayAutomatically || (!mWasSongRunning && !mIsSongRunning)) && mOutputBuffers.size() >= 1)
	{
		deallocateBuffer(mOutputBuffers.dequeue());
	}*/
}

void ViSectionHandler::finish()
{
	/*if(mAcceptFinish)
	{
		deallocateBuffer(mInputBuffers.dequeue());
		if(!mIsPlaying && mOutputBuffers.size() >= 1)
		{
			deallocateBuffer(mOutputBuffers.dequeue());
		}
		mAcceptFinish = false;
	}*/
}

void ViSectionHandler::startOutput()
{



	/*if(!mIsPlaying && mPlayAutomatically && mOutputBuffers.size() >= 1)
	{
		QObject::connect(output(), SIGNAL(finished()), this, SLOT(endOutput()));
		mIsPlaying = true;
		output()->setBuffer(mOutputBuffers.first());
		output()->start();
	}*/
}

void ViSectionHandler::endOutput()
{
	/*QObject::disconnect(output(), SIGNAL(finished()), this, SLOT(endOutput()));
	mIsPlaying = false;
	//deallocateBuffer(mOutputBuffers.dequeue());
	output()->start();
	startOutput();*/
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
	if(mSpectrumAnalyzer != NULL && mEndDetector != NULL)
	{
		QObject::connect(mSpectrumAnalyzer, SIGNAL(changed(ViRealSpectrum, qint64)), dynamic_cast<ViFrequencyEndDetector*>(mEndDetector), SLOT(addSpectrum(ViRealSpectrum)));
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

ViAudioBuffer* ViSectionHandler::allocateBuffer()
{
	return new ViAudioBuffer();
}

void ViSectionHandler::deallocateBuffer(ViAudioBuffer *buffer)
{
	delete buffer;
	buffer = NULL;
}

ViAudioObject* ViSectionHandler::currentObject()
{
	return mCurrentSongObject;
}
