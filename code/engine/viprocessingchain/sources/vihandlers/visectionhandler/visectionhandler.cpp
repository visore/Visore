#include "visectionhandler.h"
#include "vifrequencyenddetector.h"

ViSectionHandler::ViSectionHandler(ViProcessingChain *chain)
	: ViHandler(chain)
{
	mEndDetector = NULL;
	mSpectrumAnalyzer = NULL;
	mCurrentSongObject = ViAudioObject::createNull();
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
	emit recordStarted();
}

void ViSectionHandler::endRecord()
{
	QMutexLocker locker(&mMutex);
	emit recordEnded();
}

void ViSectionHandler::startSong()
{
	//QMutexLocker locker(&mMutex);
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
	emit songStarted();
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
	emit songEnded();
}

void ViSectionHandler::startInput(bool isSong)
{
	//QMutexLocker locker(&mMutex);
	ViBuffer *inputBuffer = allocateBuffer();
	ViBuffer *outputBuffer = allocateBuffer();

	ViAudioObjectPointer object = ViAudioObject::create(inputBuffer, outputBuffer);
	if(isSong)
	{
		object->setSong();
		mChain->mAudioObjects.enqueue(object);
		//mCurrentSongObject = object;
	}
	else
	{
		mNoSongObjects.enqueue(object);
	}

	input()->setBuffer(inputBuffer);
//LOG("**1: "+QString::number(object.referenceCount()));
	executor()->setObject(object);
//LOG("**2: "+QString::number(object.referenceCount()));
	executor()->initialize();
//LOG("**4: "+QString::number(object.referenceCount()));

}

void ViSectionHandler::endInput()
{
	executor()->finalize();
	/*if(!mCurrentSongObject.isNull())
	{
		mCurrentSongObject->setFinished();
		mCurrentSongObject = ViAudioObject::createNull();
	}
*/
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

ViBuffer* ViSectionHandler::allocateBuffer()
{
	return new ViBuffer();
}

void ViSectionHandler::deallocateBuffer(ViBuffer *buffer)
{
	delete buffer;
	buffer = NULL;
}

ViAudioObjectPointer ViSectionHandler::currentObject()
{
	return mCurrentSongObject;
}
