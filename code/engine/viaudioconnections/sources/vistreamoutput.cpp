#include "vistreamoutput.h"

ViStreamOutput::ViStreamOutput()
	: ViAudioOutput()
{
	mDevice = QAudioDeviceInfo::defaultOutputDevice();
	mAudioOutput = NULL;
	mVolume = 0;
	mIsMute = false;
}

ViStreamOutput::~ViStreamOutput()
{
	if(mAudioOutput != NULL)
	{
		delete mAudioOutput;
	}
}

void ViStreamOutput::setDevice(QAudioDeviceInfo device)
{
	mDevice = device;
}

void ViStreamOutput::setBuffer(ViAudioBuffer *buffer)
{
	ViAudioOutput::setBuffer(buffer);
	mBufferDevice.close();
	mBufferDevice.setBuffer(mBuffer->data());
	mBufferDevice.open(QIODevice::ReadOnly);
}

void ViStreamOutput::setFormat(ViAudioFormat format)
{
	ViAudioOutput::setFormat(format);
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

void ViStreamOutput::start()
{
	if(mAudioOutput->state() == QAudio::SuspendedState)
	{
		LOG("Playback resumed.");
		mAudioOutput->resume();
	}
	else
	{
		LOG("Playback started.");
		mAudioOutput->start(&mBufferDevice);
	}
	mState = QAudio::ActiveState;
	emit started();
}

void ViStreamOutput::stop()
{
	LOG("Playback stopped.");
	mBufferDevice.seek(0);
	mState = QAudio::StoppedState;
	mAudioOutput->stop();
	checkPosition();
	emit stopped();
}

void ViStreamOutput::pause()
{
	LOG("Playback paused.");
	mState = QAudio::SuspendedState;
	mAudioOutput->suspend();
	emit paused();
}

bool ViStreamOutput::setPosition(ViAudioPosition position)
{
	mBufferDevice.seek(position.position(ViAudioPosition::Bytes));
	checkPosition();
}

ViAudioPosition ViStreamOutput::position()
{
	if(mState == QAudio::StoppedState)
	{
		return ViAudioPosition(0, ViAudioPosition::Microseconds, mFormat);
	}
	return ViAudioPosition(mBufferDevice.pos(), ViAudioPosition::Bytes, mFormat);
}

void ViStreamOutput::checkPosition()
{
	ViAudioPosition pos = position();
	if(pos != mOldPosition)
	{
		mOldPosition = pos;
		emit positionChanged(pos);
	}
}

qreal ViStreamOutput::volume()
{
	return mAudioOutput->volume();
}

void ViStreamOutput::setVolume(qreal volumeValue)
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

void ViStreamOutput::mute(bool value)
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
