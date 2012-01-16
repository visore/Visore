#include "viaudiooutput.h"

ViAudioOutput::ViAudioOutput(ViAudioInput *input, QAudioDeviceInfo deviceInfo, QAudioFormat format)
{
	mAudioInput = input;
	mAudioOutput = NULL;
	mDeviceInfo = deviceInfo;
	mFormat = format;
	mLevelBufferLength = 0;
	mBufferPosition = 0;
}

void ViAudioOutput::setFormat(QAudioFormat format)
{
	mFormat = format;
	mLevelBufferLength = audioLength(cLevelWindowUs);
}

void ViAudioOutput::connectOutput()
{
	ViObject::connect(mAudioOutput, SIGNAL(notify()), this, SIGNAL(notify()));
	ViObject::connect(mAudioOutput, SIGNAL(stateChanged(QAudio::State)), this, SIGNAL(stateChanged(QAudio::State)));
}

void ViAudioOutput::disconnectOutput()
{
	ViObject::disconnect(mAudioOutput, SIGNAL(notify()), this, SIGNAL(notify()));
	ViObject::disconnect(mAudioOutput, SIGNAL(stateChanged(QAudio::State)), this, SIGNAL(stateChanged(QAudio::State)));
}

qint64 ViAudioOutput::audioDuration(qint64 bytes)
{
	return (bytes * 1000000) / (mFormat.frequency() * mFormat.channels() * (mFormat.sampleSize() / 8));
}

qint64 ViAudioOutput::audioLength(qint64 microSeconds)
{
	qint64 result = (mFormat.frequency() * mFormat.channels() * (mFormat.sampleSize() / 8)) * microSeconds / 1000000;
	result -= result % (mFormat.channelCount() * mFormat.sampleSize());
	return result;
}

qint64 ViAudioOutput::secondsProcessed()
{
	return mAudioOutput->processedUSecs();
}

int ViAudioOutput::levelBufferLength()
{
	return mLevelBufferLength;
}

QAudioFormat ViAudioOutput::format()
{
	return mFormat;
}

qint64 ViAudioOutput::bufferPosition()
{
	return mBufferPosition;
}

void ViAudioOutput::setBufferPosition(qint64 position)
{
	mBufferPosition = position;
}
