#include "vistreamoutput.h"

ViStreamOutput::ViStreamOutput()
	: ViAudioOutput()
{
	mDevice = QAudioDeviceInfo::defaultOutputDevice();
	mAudioOutput = NULL;
	mVolume = 0;
	mOldLength = 0;
	mIsMute = false;
	setState(QAudio::IdleState);
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

void ViStreamOutput::setBuffer(ViBuffer *buffer)
{
	ViAudioOutput::setBuffer(buffer);

	if(mBuffer == NULL)
	{
		LOG("Invalid buffer detected.");
	}
	else
	{
		mBufferDevice.close();
		mBufferDevice.setBuffer(mBuffer->data());
		mBufferDevice.open(QIODevice::ReadOnly);
	}
}

ViAudioFormat ViStreamOutput::format()
{
	if(mBuffer != NULL)
	{
		return mBuffer->format();
	}
	return ViAudioFormat();
}

void ViStreamOutput::start()
{
	if(mBuffer == NULL)
	{
		LOG("Invalid buffer detected.");
		emit finished();
		return;
	}

	if(mAudioOutput != NULL && mAudioOutput->state() == QAudio::SuspendedState)
	{
		LOG("Playback resumed.");
		mAudioOutput->resume();
	}
	else
	{
		LOG("Playback started.");
		if(mAudioOutput != NULL)
		{
			mAudioOutput->disconnect();
			delete mAudioOutput;
		}
		mAudioOutput = new QAudioOutput(mDevice, format().toQAudioFormat(), this);
		mAudioOutput->setNotifyInterval(150);
		mOldLength = 0;
		QObject::connect(mAudioOutput, SIGNAL(notify()), this, SLOT(checkPosition()), Qt::DirectConnection);
		QObject::connect(mAudioOutput, SIGNAL(stateChanged(QAudio::State)), this, SLOT(checkUnderrun()));

		mAudioOutput->start(&mBufferDevice);
	}
	setState(QAudio::ActiveState);
	emit started();
}

void ViStreamOutput::stop()
{
	if(mAudioOutput != NULL && mState != QAudio::StoppedState)
	{
		LOG("Playback stopped.");
		mBufferDevice.seek(0);
		mAudioOutput->stop();
		checkPosition();
		setState(QAudio::StoppedState);
		emit stopped();
	}
}

void ViStreamOutput::pause()
{
	if(mState != QAudio::SuspendedState)
	{
		LOG("Playback paused.");
		mAudioOutput->suspend();
		setState(QAudio::SuspendedState);
		emit paused();
	}
}

bool ViStreamOutput::setPosition(ViAudioPosition position)
{
	mBufferDevice.seek(position.position(ViAudioPosition::Bytes));
	checkPosition();
}

bool ViStreamOutput::setPosition(int seconds)
{
	setPosition(ViAudioPosition(seconds, ViAudioPosition::Seconds, format()));
}

ViAudioPosition ViStreamOutput::position()
{
	if(mBuffer != NULL)
	{
		if(mState == QAudio::StoppedState)
		{
			return ViAudioPosition(0, ViAudioPosition::Microseconds, format());
		}
		return ViAudioPosition(mBufferDevice.pos(), ViAudioPosition::Bytes, format());
	}
	return ViAudioPosition();
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

void ViStreamOutput::checkLength()
{LOG("ttttttttttttttttt");
	if(buffer()->size() != mOldLength)
	{
		mOldLength = buffer()->size();
		emit lengthChanged(ViAudioPosition(mOldLength, ViAudioPosition::Bytes, format()));
	}
}

void ViStreamOutput::checkUnderrun()
{
	if(mAudioOutput->error() == QAudio::UnderrunError)
	{
		LOG("Audio output underrun.", QtWarningMsg);
		pause();
		emit underrun();
	}
}

qreal ViStreamOutput::volume()
{
	if(mAudioOutput != NULL)
	{
		return mAudioOutput->volume();
	}
	return -1;
}

void ViStreamOutput::setVolume(qreal volumeValue)
{
	if(mIsMute)
	{
		mMuteVolume = volume();
	}
	else if(mAudioOutput != NULL)
	{
		mAudioOutput->setVolume(volumeValue);
	}
	emit volumeChanged(volume());
}

void ViStreamOutput::mute(bool value)
{
	if(!mIsMute && value)
	{
		mMuteVolume = volume();
		setVolume(0);
		mIsMute = true;
		emit muted();
		emit volumeChanged(volume());
	}
	else if(!value)
	{
		unmute();
	}
}

void ViStreamOutput::unmute()
{
	if(mIsMute)
	{
		mIsMute = false;
		setVolume(mMuteVolume);
		emit unmuted();
		emit volumeChanged(volume());
	}
}
