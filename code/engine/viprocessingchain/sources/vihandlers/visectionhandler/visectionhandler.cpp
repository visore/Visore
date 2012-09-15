#include "visectionhandler.h"
#include "vifrequencyenddetector.h"

ViSectionHandler::ViSectionHandler(ViProcessingChain *chain)
	: ViHandler(chain)
{
	mEndDetector = NULL;
	mSpectrumAnalyzer = NULL;
	mIsSongRunning = false;
	mWasSongRunning = false;
	QObject::connect(mChain, SIGNAL(attached(ViProcessor*)), this, SLOT(setDetector(ViProcessor*)));

	mInputBuffer = NULL;
	mOutputBuffer = NULL;
	mAcceptFinish = false;
	mPlayAutomatically = true;
	mIsPlaying = false;
	QObject::connect(mChain, SIGNAL(inputChanged()), this, SLOT(startInput()));
}

bool ViSectionHandler::isSongRunning()
{
	return mIsSongRunning;
}

bool ViSectionHandler::wasSongRunning()
{
	return mWasSongRunning;
}

void ViSectionHandler::startRecord()
{
	//mChain->startInput();
	emit mChain->recordStarted();
}

void ViSectionHandler::endRecord()
{
	emit mChain->recordEnded();
}

void ViSectionHandler::startSong()
{
	endInput();
	startInput();
	startOutput();




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

void ViSectionHandler::startInput()
{
	mInputBuffer = allocateBuffer(ViAudio::AudioInput);
	mOutputBuffer = allocateBuffer(ViAudio::AudioOutput);

	input()->setBuffer(mInputBuffer);
	executor()->setBuffer(ViAudio::AudioInput, mInputBuffer);
	executor()->setBuffer(ViAudio::AudioOutput, mOutputBuffer);
	executor()->initialize();
	mOutputBuffers.enqueue(mOutputBuffer);
}

void ViSectionHandler::endInput()
{
	executor()->finalize();
	if(mInputBuffer != NULL)
	{
		mInputBuffers.enqueue(mInputBuffer);
	}
	if((!mPlayAutomatically || (!mWasSongRunning && !mIsSongRunning)) && mOutputBuffers.size() >= 1)
	{
			deallocateBuffer(mOutputBuffers.dequeue());
	}
}

void ViSectionHandler::finish()
{
	if(mAcceptFinish)
	{
		deallocateBuffer(mInputBuffers.dequeue());
		if(!mIsPlaying && mOutputBuffers.size() >= 1)
		{
			deallocateBuffer(mOutputBuffers.dequeue());
		}
		mAcceptFinish = false;
	}
}

void ViSectionHandler::startOutput()
{
	if(!mIsPlaying && mPlayAutomatically && mOutputBuffers.size() >= 1)
	{
		QObject::connect(output(), SIGNAL(finished()), this, SLOT(endOutput()));
		mIsPlaying = true;
		output()->setBuffer(mOutputBuffers.first());
		output()->start();
	}
}

void ViSectionHandler::endOutput()
{
	QObject::disconnect(output(), SIGNAL(finished()), this, SLOT(endOutput()));
	mIsPlaying = false;
	deallocateBuffer(mOutputBuffers.dequeue());
	output()->start();
	startOutput();
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
			//checkInput();
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

ViAudioBuffer* ViSectionHandler::allocateBuffer(ViAudio::Mode mode)
{
	ViAudioBuffer *buffer = new ViAudioBuffer();
	if(mode == ViAudio::AudioInput)
	{
		//mInputBuffers.enqueue(buffer);
	}
	else if(mode == ViAudio::AudioOutput)
	{
		//mOutputBuffers.enqueue(buffer);
	}
	return buffer;
}

void ViSectionHandler::deallocateBuffer(ViAudioBuffer *buffer)
{
	delete buffer;
	buffer = NULL;
}

ViAudioBuffer* ViSectionHandler::nextBuffer(ViAudio::Mode mode)
{
	if(mode == ViAudio::AudioInput)
	{
		if(mInputBuffer != NULL)
		{
			delete mInputBuffer;
		}
		if(mInputBuffers.isEmpty())
		{
			allocateBuffer(ViAudio::AudioInput);
		}
		mInputBuffer = mInputBuffers.dequeue();
		return mInputBuffer;
	}
	else if(mode == ViAudio::AudioOutput)
	{
		if(mOutputBuffer != NULL)
		{
			delete mOutputBuffer;
		}
		if(mOutputBuffers.isEmpty())
		{
			allocateBuffer(ViAudio::AudioOutput);
		}
		mOutputBuffer = mOutputBuffers.dequeue();
		return mOutputBuffer;
	}
}
