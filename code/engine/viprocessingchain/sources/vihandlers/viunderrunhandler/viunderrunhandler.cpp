#include "viunderrunhandler.h"

#define DEFAULT_SONG_LENGTH 240000
#define DEFAULT_BUFFERING_SECONDS 5

ViUnderrunHandler::ViUnderrunHandler(ViProcessingChain *chain)
	: ViHandler(chain)
{
	mTimer.setInterval(1000);
	QObject::connect(&mTimer, SIGNAL(timeout()), this, SLOT(update()));
	QObject::connect(chain, SIGNAL(streamOutputChanged(ViStreamOutput*)), this, SLOT(changeOutput(ViStreamOutput*)));
}

void ViUnderrunHandler::changeOutput(ViStreamOutput *output)
{
	QObject::disconnect(this, SLOT(handleUnderrun()));
	QObject::connect(output, SIGNAL(underrun()), this, SLOT(handleUnderrun()));
}

void ViUnderrunHandler::handleUnderrun()
{
	LOG("***1");
	ViAudioObject *object = mChain->playingObject();
	if(object != NULL)
	{
		LOG("***2");
		if(object->isFinished())
		{
			LOG("***3");
			mChain->mStreamOutput->stop();
		}
		else if(!mBufferingStarted)
		{
			LOG("***4");
			mSecondsPassed = 0;
			mSecondsNeeded = 0;
			mBufferingStarted = false;
			QObject::connect(&mChain->mMultiExecutor, SIGNAL(processingRateChanged(qreal)), this, SLOT(calculate(qreal)));
			calculate(mChain->mMultiExecutor.processingRate());
		}
		LOG("***5");
	}
	LOG("***6");
}

void ViUnderrunHandler::calculate(qreal processingRate)
{
	
	mMutex.lock();
	if(processingRate < 1)
	{
		mSecondsNeeded = DEFAULT_BUFFERING_SECONDS;
	}
	mSecondsNeeded = secondsNeeded(processingRate);
	if(mSecondsNeeded <= 0)
	{
		mSecondsNeeded = DEFAULT_BUFFERING_SECONDS;
	}
	if(!mBufferingStarted)
	{
		mTimer.start();
		emit started();
		mBufferingStarted = true;
		LOG("Buffering started (processing rate: " + QString::number(processingRate, 'f', 1) + "Hz, buffer needed: " + QString::number(mSecondsNeeded) + "s)");
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
		QObject::disconnect(&mChain->mMultiExecutor, SIGNAL(processingRateChanged(qreal)), this, SLOT(calculate(qreal)));
		emit finished();
		mChain->mStreamOutput->start();
	}
}

int ViUnderrunHandler::secondsNeeded(qreal processingRate)
{
	ViAudioFormat format = mChain->mStreamOutput->format();
	qreal ratio = 1 - (processingRate / format.sampleRate());
	ratio *= 1 + ratio;
	qint64 bytesNeeded = ViAudioPosition::convertPosition(DEFAULT_SONG_LENGTH, ViAudioPosition::Milliseconds, ViAudioPosition::Bytes, format);
	bytesNeeded -= mChain->mStreamOutput->position().position(ViAudioPosition::Bytes);
	bytesNeeded *= ratio;
	return ceil(ViAudioPosition::convertPosition(bytesNeeded, ViAudioPosition::Bytes, ViAudioPosition::Seconds, format));
}
