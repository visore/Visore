#include "vistreaminput.h"

ViStreamBuffer::ViStreamBuffer(ViBuffer *audioBuffer)
	: QBuffer()
{
	mReadStream = new QDataStream(&buffer(), QIODevice::ReadOnly);
	mWriteStream = audioBuffer->createWriteStream();
	QObject::connect(this, SIGNAL(bytesWritten(qint64)), this, SLOT(addData(qint64)));
	open(QIODevice::WriteOnly);
}

ViStreamBuffer::~ViStreamBuffer()
{
	delete mReadStream;
	close();
	clear();
}

void ViStreamBuffer::addData(qint64 bytes)
{
	char data[bytes];
	mReadStream->readRawData(data, bytes);
	mWriteStream->write(data, bytes);
	buffer().remove(0, bytes);
}

void ViStreamBuffer::clear()
{
	buffer().clear();
}

ViStreamInput::ViStreamInput()
	: ViAudioInput()
{
	mDevice = QAudioDeviceInfo::defaultInputDevice();
	mAudioInput = NULL;
	mBufferDevice = NULL;
	QObject::connect(&mTimer, SIGNAL(timeout()), this, SLOT(checkBuffer()));
	setState(QAudio::IdleState);
}

ViStreamInput::~ViStreamInput()
{
	if(mAudioInput != NULL)
	{
		delete mAudioInput;
	}
	if(mBufferDevice != NULL)
	{
		delete mBufferDevice;
	}
}

void ViStreamInput::checkBuffer()
{
	if(mPreviousSize == mBufferDevice->size())
	{
		stopChecking();
		LOG("The audio input has stop feeding data to the buffer.", QtFatalMsg);
	}
	mPreviousSize = mBufferDevice->size();
}

void ViStreamInput::startChecking()
{
	mPreviousSize = 0;
	mTimer.start(1000);
}

void ViStreamInput::stopChecking()
{
	mTimer.stop();
}

void ViStreamInput::setDevice(QAudioDeviceInfo device)
{
	mDevice = device;
}

void ViStreamInput::setBuffer(ViBuffer *buffer)
{
	bool startAgain = (state() == QAudio::ActiveState);
	stop();
	setState(QAudio::IdleState);

	ViAudioInput::setBuffer(buffer);
	mBuffer->setFormat(mFormat);

	if(startAgain)
	{
		start();
	}
}

ViAudioFormat ViStreamInput::format()
{
	return mFormat;
}

void ViStreamInput::setFormat(ViAudioFormat format)
{
	mFormat = format;
	if(mBuffer != NULL)
	{
		mBuffer->setFormat(mFormat);
	}
}

void ViStreamInput::start()
{
	if(mAudioInput == NULL)
	{
		mAudioInput = new QAudioInput(mDevice, mFormat.toQAudioFormat(), this);
	}
	if(mAudioInput->state() == QAudio::SuspendedState)
	{
		LOG("Recording resumed.");
		mAudioInput->resume();
	}
	else if(state() != QAudio::ActiveState)
	{
		LOG("Recording started.");
		if(mBufferDevice != NULL)
		{
			delete mBufferDevice;
		}
		mBufferDevice = new ViStreamBuffer(mBuffer);
		if(mAudioInput != NULL)
		{
			delete mAudioInput;
		}
		mAudioInput = new QAudioInput(mDevice, mFormat.toQAudioFormat(), this);
		mAudioInput->start(mBufferDevice);
	}
	startChecking();
	setState(QAudio::ActiveState);
}

void ViStreamInput::stop()
{
	if(mAudioInput != NULL && mBufferDevice != NULL && state() != QAudio::StoppedState)
	{
		stopChecking();
		mAudioInput->stop();
		delete mBufferDevice;
		mBufferDevice = NULL;
		delete mAudioInput;
		mAudioInput = NULL;
		setState(QAudio::StoppedState);
		LOG("Recording stopped.");
	}
}

void ViStreamInput::pause()
{
	LOG("Recording paused.");
	mAudioInput->suspend();
	stopChecking();
	setState(QAudio::SuspendedState);
}

qreal ViStreamInput::volume()
{
	return mAudioInput->volume();
}

void ViStreamInput::setVolume(qreal volumeValue)
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

void ViStreamInput::mute(bool value)
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
