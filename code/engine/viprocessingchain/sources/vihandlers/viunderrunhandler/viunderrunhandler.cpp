#include "viunderrunhandler.h"

#define DEFAULT_SONG_LENGTH 240000

ViUnderrunHandler::ViUnderrunHandler(ViProcessingChain *chain)
	: ViHandler(chain)
{
	mOutput = NULL;
	mTimer.setInterval(1000);
	QObject::connect(&mTimer, SIGNAL(timeout()), this, SLOT(update()));
	QObject::connect(chain, SIGNAL(streamOutputChanged(ViStreamOutput*)), this, SLOT(changeStreamOutput(ViStreamOutput*)));
}

void ViUnderrunHandler::changeStreamOutput(ViStreamOutput *output)
{
	QObject::disconnect(this, SLOT(handleUnderrun()));
	if(output != NULL)
	{
		mFormat = output->format();
		mOutput = output;
		QObject::connect(output, SIGNAL(underrun()), this, SLOT(handleUnderrun()));
	}
}

void ViUnderrunHandler::handleUnderrun()
{
	mSecondsPassed = 0;
	mSecondsNeeded = 0;
	mBufferingStarted = false;
	QObject::connect(mChain->executor(), SIGNAL(processingRateChanged(qreal)), this, SLOT(calculate(qreal)));
	calculate(mChain->executor()->processingRate());
}

void ViUnderrunHandler::calculate(qreal processingRate)
{
	mMutex.lock();
	mSecondsNeeded = secondsNeeded(processingRate);
	if(!mBufferingStarted)
	{	
		if(mSecondsNeeded <= 0)
		{
			QObject::disconnect(mChain->executor(), SIGNAL(processingRateChanged(qreal)), this, SLOT(calculate(qreal)));
			mOutput->start();
		}
		else
		{
			mTimer.start();
			emit started();
			mBufferingStarted = true;
			LOG("Buffering started (processing rate: " + QString::number(processingRate, 'f', 1) + "Hz, buffer needed: " + QString::number(mSecondsNeeded) + "s)");
		}
	}
	mMutex.unlock();
}

void ViUnderrunHandler::update()
{
	++mSecondsPassed;
	mMutex.lock();
	short progress = mSecondsPassed / (mSecondsNeeded / 100.0);
	mMutex.unlock();
	emit progressed(progress);
	if(progress >= 100)
	{
		mTimer.stop();
		QObject::disconnect(mChain->executor(), SIGNAL(processingRateChanged(qreal)), this, SLOT(calculate(qreal)));
		emit finished();
		mOutput->start();
	}
}

int ViUnderrunHandler::secondsNeeded(qreal processingRate)
{
	qreal ratio = 1 - (processingRate / mFormat.sampleRate());
	ratio *= 1 + ratio;
	qint64 bytesNeeded = ViAudioPosition::convertPosition(DEFAULT_SONG_LENGTH, ViAudioPosition::Milliseconds, ViAudioPosition::Bytes, mFormat);
	bytesNeeded -= mChain->streamOutput()->position().position(ViAudioPosition::Bytes);
	bytesNeeded *= ratio;
	return ceil(ViAudioPosition::convertPosition(bytesNeeded, ViAudioPosition::Bytes, ViAudioPosition::Seconds, mFormat));
}
