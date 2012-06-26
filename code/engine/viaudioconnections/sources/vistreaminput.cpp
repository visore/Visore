#include "vistreaminput.h"

ViStreamBuffer::ViStreamBuffer()
	: QBuffer()
{
}

void ViStreamBuffer::setBuffer(ViAudioBuffer *buffer)
{
	mStream = buffer->createWriteStream();
}

qint64 ViStreamBuffer::write(const char *data, qint64 maxSize)
{
	return mStream->write((char*) data, maxSize);
}

qint64 ViStreamBuffer::write(const char *data)
{
	return mStream->write((char*) data, qstrlen(data));
}

qint64 ViStreamBuffer::write(const QByteArray &byteArray)
{
	return mStream->write((char*) byteArray.data(), byteArray.size());
}

qint64 ViStreamBuffer::writeData(const char *data, qint64 length)
{
	return mStream->write((char*) data, length);
}

ViStreamInput::ViStreamInput()
	: ViAudioInput()
{
	mDevice = QAudioDeviceInfo::defaultInputDevice();
	mAudioInput = NULL;
	setState(QAudio::IdleState);
}

ViStreamInput::~ViStreamInput()
{
	if(mAudioInput != NULL)
	{
		delete mAudioInput;
	}
}

void ViStreamInput::setDevice(QAudioDeviceInfo device)
{
	mDevice = device;
}

void ViStreamInput::setBuffer(ViAudioBuffer *buffer)
{
	ViAudioInput::setBuffer(buffer);
	mBuffer->setFormat(mFormat);
	mBufferDevice.close();
	mBufferDevice.setBuffer(mBuffer);
	mBufferDevice.open(QIODevice::WriteOnly);
}

void ViStreamInput::setFormat(ViAudioFormat format)
{
	ViAudioInput::setFormat(format);
	if(mBuffer != NULL)
	{
		mBuffer->setFormat(mFormat);
	}
	if(mAudioInput != NULL)
	{
		delete mAudioInput;
	}
	mAudioInput = new QAudioInput(mDevice, mFormat, this);
}

void ViStreamInput::start()
{
	if(mAudioInput->state() == QAudio::SuspendedState)
	{
		LOG("Recordingresumed.");
		mAudioInput->resume();
	}
	else
	{
		LOG("Recording started.");
		mAudioInput->start(&mBufferDevice);
	}
	setState(QAudio::ActiveState);
}

void ViStreamInput::stop()
{
	LOG("Recording stopped.");
	mBufferDevice.seek(0);
	mAudioInput->stop();
	setState(QAudio::StoppedState);
}

void ViStreamInput::pause()
{
	LOG("Recording paused.");
	mAudioInput->suspend();
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
