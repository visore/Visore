#include "viqtstreamoutput.h"

ViQtStreamOutput::ViQtStreamOutput(ViAudioBuffer *buffer, ViAudioMetaData *metaData, ViAudioDevice *device)
	: ViStreamOutput(buffer, metaData, device)
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

	ViObject::connect(mAudioOutput, SIGNAL(notify()), this, SLOT(checkPosition()));
}

ViQtStreamOutput::~ViQtStreamOutput()
{
	delete mStream;
	delete mAudioOutput;
	delete mBufferDevice;
}

void ViQtStreamOutput::initialize()
{

}

void ViQtStreamOutput::free()
{

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
}

void ViQtStreamOutput::pause()
{
	mAudioOutput->suspend();
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
	return ViAudioPosition(mAudioOutput->elapsedUSecs(), format);
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

}

void ViQtStreamOutput::setVolume(qreal volume)
{

}

void ViQtStreamOutput::mute(bool value)
{

}
