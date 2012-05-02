#include "viqtstreaminput.h"

ViQtStreamInput::ViQtStreamInput()
	: ViStreamInput()
{
}

ViQtStreamInput::~ViQtStreamInput()
{
	free();
}

void ViQtStreamInput::initialize()
{
	ViStreamInput::initialize();

	mBufferDevice = new QBuffer(mBuffer->data(), this);
	mBufferDevice->open(QIODevice::WriteOnly);

	mAudioInput = new QAudioInput(mDevice, mFormat, this);
	mAudioInput->setNotifyInterval(25);
	ViObject::connect(mAudioInput, SIGNAL(notify()), this, SLOT(a()));
}

void ViQtStreamInput::a()
{
	//cout<<"AAAAAAAAAA: "<<mBufferDevice->size()<<<<endl;
}

void ViQtStreamInput::free()
{
	ViStreamInput::free();
	delete mAudioInput;
	delete mBufferDevice;
}

void ViQtStreamInput::start()
{
	if(mAudioInput->state() == QAudio::SuspendedState)
	{
		mAudioInput->resume();
	}
	else
	{
		mAudioInput->start(mBufferDevice);
	}
	mState = QAudio::ActiveState;
}

void ViQtStreamInput::stop()
{
	mBufferDevice->seek(0);
	mState = QAudio::StoppedState;
	mAudioInput->stop();
}

void ViQtStreamInput::pause()
{
	mState = QAudio::SuspendedState;
	mAudioInput->suspend();
}

qreal ViQtStreamInput::volume()
{
	return mAudioInput->volume();
}

void ViQtStreamInput::setVolume(qreal volumeValue)
{
	if(mIsMute)
	{
		mMuteVolume = volume();
	}
	else
	{
		mAudioInput->setVolume(volumeValue);
	}
}

void ViQtStreamInput::mute(bool value)
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
