#include "viqtstreaminput.h"

#include <iostream>
using namespace std;

ViQtStreamBuffer::ViQtStreamBuffer(ViAudioBuffer *buffer)
	: QBuffer()
{
	mStream = buffer->createWriteStream();
}

qint64 ViQtStreamBuffer::write(const char *data, qint64 maxSize)
{
	return mStream->write((char*) data, maxSize);
}

qint64 ViQtStreamBuffer::write(const char *data)
{
	return mStream->write((char*) data, qstrlen(data));
}

qint64 ViQtStreamBuffer::write(const QByteArray &byteArray)
{
	return mStream->write((char*) byteArray.data(), byteArray.size());
}

qint64 ViQtStreamBuffer::writeData(const char *data, qint64 length)
{
	return mStream->write((char*) data, length);
}

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

	mBufferDevice = new ViQtStreamBuffer(mBuffer);
	mBufferDevice->open(QIODevice::WriteOnly);

	mBuffer->setFormat(mFormat);
	mAudioInput = new QAudioInput(mDevice, mFormat, this);
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
