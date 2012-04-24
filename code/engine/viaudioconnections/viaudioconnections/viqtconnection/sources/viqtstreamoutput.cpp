#include "viqtstreamoutput.h"
#include "vicodecmanager.h"

ViQtStreamOutput::ViQtStreamOutput()
	: ViStreamOutput()
{
}

ViQtStreamOutput::~ViQtStreamOutput()
{
	free();
}

void ViQtStreamOutput::initialize()
{
	ViStreamOutput::initialize();

	mBufferDevice = new QBuffer(mBuffer->data(), this);
	mBufferDevice->open(QIODevice::ReadOnly);

	mFormat.setCodec(ViCodecManager::selected("PCM"));
	mFormat.setSampleRate(44100);
	mFormat.setSampleSize(16);
	mFormat.setChannelCount(2);
	mAudioOutput = new QAudioOutput(mDevice, mFormat, this);
	mAudioOutput->setNotifyInterval(25);

	/*if(!mDevice.isFormatSupported(mFormat))
	{
		setErrorParameters("ViQtStreamOutput", "The format is not supported on this device", ViErrorInfo::NonFatal);
	}*/

	ViObject::connect(mAudioOutput, SIGNAL(notify()), this, SLOT(checkPosition()));

	mMuteVolume = volume();
	mIsMute = false;
}

void ViQtStreamOutput::free()
{
	ViStreamOutput::free();
	delete mAudioOutput;
	delete mBufferDevice;
}

void ViQtStreamOutput::start()
{
	if(mAudioOutput->state() == QAudio::SuspendedState)
	{
		mAudioOutput->resume();
	}
	else
	{
		mAudioOutput->start(mBufferDevice);
	}
	mState = QAudio::ActiveState;
}

void ViQtStreamOutput::stop()
{
	mBufferDevice->seek(0);
	mState = QAudio::StoppedState;
	mAudioOutput->stop();
	checkPosition();
}

void ViQtStreamOutput::pause()
{
	mState = QAudio::SuspendedState;
	mAudioOutput->suspend();
}

bool ViQtStreamOutput::setPosition(ViAudioPosition position)
{
	mBufferDevice->seek(position.position(ViAudioPosition::Bytes));
	checkPosition();
}

ViAudioPosition ViQtStreamOutput::position()
{
	if(mState == QAudio::StoppedState)
	{
		return ViAudioPosition(0, ViAudioPosition::Microseconds, mFormat);
	}
	return ViAudioPosition(mBufferDevice->pos(), ViAudioPosition::Bytes, mFormat);
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
