#include "viqtstreamoutput.h"

ViQtStreamOutput::ViQtStreamOutput(ViAudioFormat format, ViAudioBuffer *buffer, ViAudioDevice *device)
	: ViStreamOutput(format, buffer, device)
{
	initialize();
}

ViQtStreamOutput::~ViQtStreamOutput()
{
	free();
}

void ViQtStreamOutput::initialize()
{
	mStream = mBuffer->createReadStream();
	mBufferDevice = new QBuffer(mBuffer->data(), this);
	mBufferDevice->open(QIODevice::ReadOnly);

	QAudioFormat format;
	format.setSampleSize(16);
	format.setSampleRate(44100);
	format.setCodec("audio/pcm");
	format.setChannelCount(2);
	format.setByteOrder(QAudioFormat::LittleEndian);
	format.setSampleType(QAudioFormat::SignedInt);

	mAudioOutput = new QAudioOutput(QAudioDeviceInfo::defaultOutputDevice(), format, this);
	mAudioOutput->setVolume(100);
	mAudioOutput->setNotifyInterval(50);


     QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
     if (!info.isFormatSupported(format)) {
         cout << "raw audio format not supported by backend, cannot play audio."<<endl;
     }

	mProcessedMicroseconds = 0;
	ViObject::connect(mAudioOutput, SIGNAL(notify()), this, SLOT(checkPosition()));

	mMuteVolume = volume();
	mIsMute = false;
}

void ViQtStreamOutput::free()
{
	delete mStream;
	delete mAudioOutput;
	delete mBufferDevice;
}

void ViQtStreamOutput::start()
{
	if(mAudioOutput->state() == QAudio::ActiveState)
	{
		mAudioOutput->resume();
	}
	else
	{
		mAudioOutput->start(mBufferDevice);
	}
}

void ViQtStreamOutput::stop()
{
	mAudioOutput->stop();
	mProcessedMicroseconds = 0;
}

void ViQtStreamOutput::pause()
{
	mAudioOutput->suspend();
	mProcessedMicroseconds = position().microseconds();
}

bool ViQtStreamOutput::setPosition(ViAudioPosition position)
{
}

ViAudioPosition ViQtStreamOutput::position()
{
	QAudioFormat format;
	format.setSampleSize(16);
	format.setSampleRate(44100);
	format.setCodec("audio/pcm");
	format.setChannelCount(2);
	format.setByteOrder(QAudioFormat::LittleEndian);
	format.setSampleType(QAudioFormat::SignedInt);
	return ViAudioPosition(mAudioOutput->processedUSecs() + mProcessedMicroseconds, format);
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
