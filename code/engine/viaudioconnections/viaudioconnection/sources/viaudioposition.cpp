#include "viaudioposition.h"

ViAudioPosition::ViAudioPosition()
{
	mMicroseconds = 0;
	mFormat = QAudioFormat();
}

ViAudioPosition::ViAudioPosition(qreal microseconds, ViAudioPosition::Unit unit, QAudioFormat format)
{
	setPosition(microseconds, unit, format);
}

void ViAudioPosition::setPosition(qreal position, ViAudioPosition::Unit unit, QAudioFormat format)
{
	mFormat = format;
	setPosition(position, unit);
}

void ViAudioPosition::setPosition(qreal position, ViAudioPosition::Unit unit)
{
	if(unit == ViAudioPosition::Microseconds)
	{
		mMicroseconds = position;
	}
	else if(unit == ViAudioPosition::Milliseconds)
	{
		mMicroseconds = position * 1000.0;
	}
	else if(unit == ViAudioPosition::Seconds)
	{
		mMicroseconds = position * 1000000.0;
	}
	else if(unit == ViAudioPosition::Samples)
	{
		mMicroseconds = position / (mFormat.sampleRate() * mFormat.channelCount()) * 1000000.0;
	}
	else if(unit == ViAudioPosition::Bytes)
	{
		mMicroseconds = position / (mFormat.sampleRate() * mFormat.channelCount() * (mFormat.sampleSize() / 8)) * 1000000.0;
	}
}

qreal ViAudioPosition::position(ViAudioPosition::Unit unit)
{
	if(unit == ViAudioPosition::Microseconds)
	{
		return mMicroseconds;
	}
	else if(unit == ViAudioPosition::Milliseconds)
	{
		return mMicroseconds / 1000.0;
	}
	else if(unit == ViAudioPosition::Seconds)
	{
		return mMicroseconds / 1000000.0;
	}
	else if(unit == ViAudioPosition::Samples)
	{
		return (mMicroseconds / 1000000.0) * mFormat.sampleRate() * mFormat.channelCount();
	}
	else if(unit == ViAudioPosition::Bytes)
	{
		return (mMicroseconds / 1000000.0) * mFormat.sampleRate() * mFormat.channelCount() * (mFormat.sampleSize() / 8);
	}
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
