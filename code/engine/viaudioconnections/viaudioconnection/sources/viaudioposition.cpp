#include "viaudioposition.h"

ViAudioPosition::ViAudioPosition()
{
	mMicroseconds = 0;
	mFormat = QAudioFormat();
}

ViAudioPosition::ViAudioPosition(qint64 microseconds, QAudioFormat format)
{
	initialize(microseconds, format);
}

void ViAudioPosition::initialize(qint64 microseconds, QAudioFormat format)
{
	mMicroseconds = microseconds;
	mFormat = format;
}

qreal ViAudioPosition::seconds()
{
	return milliseconds() / 1000.0;
}

qreal ViAudioPosition::milliseconds()
{
	return mMicroseconds / 1000.0;
}

qint64 ViAudioPosition::microseconds()
{
	return mMicroseconds;
}

qint64 ViAudioPosition::bytes()
{
	return sample() * mFormat.sampleSize();
}

qint64 ViAudioPosition::sample()
{
	return mFormat.sampleRate() * seconds() * mFormat.channelCount();
}

QAudioFormat ViAudioPosition::format()
{
	return mFormat;
}

bool ViAudioPosition::operator ==(const ViAudioPosition &other) const
{
	return mMicroseconds == other.mMicroseconds;
}

bool ViAudioPosition::operator !=(const ViAudioPosition &other) const
{
	return mMicroseconds != other.mMicroseconds;
}
