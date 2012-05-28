#include "viqtstreaminput.h"

#include <iostream>
using namespace std;

ViQtStreamBuffer::ViQtStreamBuffer()
	: QBuffer()
{
}

void ViQtStreamBuffer::setBuffer(ViAudioBuffer *buffer)
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
	mAudioInput = NULL;
}

ViQtStreamInput::~ViQtStreamInput()
{
	if(mAudioInput != NULL)
	{
		delete mAudioInput;
	}
}

void ViQtStreamInput::setBuffer(ViAudioBuffer *buffer)
{
	ViStreamInput::setBuffer(buffer);
	mBufferDevice.close();
	mBufferDevice.setBuffer(mBuffer);
	mBufferDevice.open(QIODevice::WriteOnly);
}

void ViQtStreamInput::setFormat(ViAudioFormat format)
{
	ViStreamInput::setFormat(format);
	mBuffer->setFormat(mFormat);
	if(mAudioInput != NULL)
	{
		delete mAudioInput;
	}
	mAudioInput = new QAudioInput(mDevice, mFormat, this);
}

void ViQtStreamInput::start()
{
	if(mAudioInput->state() == QAudio::SuspendedState)
	{
		mAudioInput->resume();
	}
	else
	{
		mAudioInput->start(&mBufferDevice);
	}
	mState = QAudio::ActiveState;
	emit started();
}

void ViQtStreamInput::stop()
{
	mBufferDevice.seek(0);
	mState = QAudio::StoppedState;
	mAudioInput->stop();
	emit stopped();
}

void ViQtStreamInput::pause()
{
	mState = QAudio::SuspendedState;
	mAudioInput->suspend();
	emit paused();
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
