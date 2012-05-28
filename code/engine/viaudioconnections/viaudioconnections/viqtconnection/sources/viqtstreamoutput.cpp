#include "viqtstreamoutput.h"
#include "vicodecmanager.h"

ViQtStreamOutput::ViQtStreamOutput()
	: ViStreamOutput()
{
	mAudioOutput = NULL;
	mIsMute = false;
}

ViQtStreamOutput::~ViQtStreamOutput()
{
	if(mAudioOutput != NULL)
	{
		delete mAudioOutput;
	}
}

void ViQtStreamOutput::setBuffer(ViAudioBuffer *buffer)
{
	ViStreamOutput::setBuffer(buffer);
	mBufferDevice.close();
	mBufferDevice.setBuffer(mBuffer->data());
	mBufferDevice.open(QIODevice::ReadOnly);
}

void ViQtStreamOutput::setFormat(ViAudioFormat format)
{
	ViStreamOutput::setFormat(format);
	mBuffer->setFormat(mFormat);
	if(mAudioOutput != NULL)
	{
		QObject::disconnect(mAudioOutput, SIGNAL(notify()), this, SLOT(checkPosition()));
		delete mAudioOutput;
	}
	mAudioOutput = new QAudioOutput(mDevice, mFormat, this);
	mAudioOutput->setNotifyInterval(25);
	QObject::connect(mAudioOutput, SIGNAL(notify()), this, SLOT(checkPosition()));
}

void ViQtStreamOutput::start()
{
	if(mAudioOutput->state() == QAudio::SuspendedState)
	{
		mAudioOutput->resume();
	}
	else
	{
		mAudioOutput->start(&mBufferDevice);
	}
	mState = QAudio::ActiveState;
	emit started();
}

void ViQtStreamOutput::stop()
{
	mBufferDevice.seek(0);
	mState = QAudio::StoppedState;
	mAudioOutput->stop();
	checkPosition();
	emit stopped();
}

void ViQtStreamOutput::pause()
{
	mState = QAudio::SuspendedState;
	mAudioOutput->suspend();
	emit paused();
}

bool ViQtStreamOutput::setPosition(ViAudioPosition position)
{
	mBufferDevice.seek(position.position(ViAudioPosition::Bytes));
	checkPosition();
}

ViAudioPosition ViQtStreamOutput::position()
{
	if(mState == QAudio::StoppedState)
	{
		return ViAudioPosition(0, ViAudioPosition::Microseconds, mFormat);
	}
	return ViAudioPosition(mBufferDevice.pos(), ViAudioPosition::Bytes, mFormat);
}

void ViQtStreamOutput::checkPosition()
{
	ViAudioPosition pos = position();
	if(pos != mOldPosition)
	{
		mOldPosition = pos;
		emit positionChanged(pos);
	}
}

qreal ViQtStreamOutput::volume()
{
	return mAudioOutput->volume();
}

void ViQtStreamOutput::setVolume(qreal volumeValue)
{
	if(mIsMute)
	{
		mMuteVolume = volume();
	}
	else
	{
		mAudioOutput->setVolume(volumeValue);
	}
}

void ViQtStreamOutput::mute(bool value)
{
	if(value)
	{
		mMuteVolume = volume();
		setVolume(0);
		mIsMute = true;
	}
	else
	{
		mIsMute = false;
		setVolume(mMuteVolume);
	}
}
