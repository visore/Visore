#include "viaudioposition.h"

ViAudioPosition::ViAudioPosition()
{
	mMicroseconds = 0;
	mFormat = QAudioFormat();
}

ViAudioPosition::ViAudioPosition(const qreal value, const ViAudioPosition::Unit unit, const QAudioFormat format)
{
	setPosition(value, unit, format);
}

qreal ViAudioPosition::convertPosition(const qreal position, const ViAudioPosition::Unit fromUnit, const ViAudioPosition::Unit toUnit, const QAudioFormat &format)
{
	qreal microseconds = 0;

	if(fromUnit == ViAudioPosition::Microseconds)
	{
		microseconds = position;
	}
	else if(fromUnit == ViAudioPosition::Milliseconds)
	{
		microseconds = position * 1000.0;
	}
	else if(fromUnit == ViAudioPosition::Seconds)
	{
		microseconds = position * 1000000.0;
	}
	else if(fromUnit == ViAudioPosition::Samples)
	{
		microseconds = position / (format.sampleRate() * format.channelCount()) * 1000000.0;
	}
	else if(fromUnit == ViAudioPosition::Bytes)
	{
		microseconds = position / (format.sampleRate() * format.channelCount() * (format.sampleSize() / 8)) * 1000000.0;
	}

	if(toUnit == ViAudioPosition::Microseconds)
	{
		return microseconds;
	}
	else if(toUnit == ViAudioPosition::Milliseconds)
	{
		return microseconds / 1000.0;
	}
	else if(toUnit == ViAudioPosition::Seconds)
	{
		return microseconds / 1000000.0;
	}
	else if(toUnit == ViAudioPosition::Samples)
	{
		return (microseconds / 1000000.0) * format.sampleRate() * format.channelCount();
	}
	else if(toUnit == ViAudioPosition::Bytes)
	{
		return (microseconds / 1000000.0) * format.sampleRate() * format.channelCount() * (format.sampleSize() / 8);
	}
}

void ViAudioPosition::setPosition(const qreal position, const ViAudioPosition::Unit unit, const QAudioFormat format)
{
	mFormat = format;
	setPosition(position, unit);
}

void ViAudioPosition::setPosition(const qreal position, const ViAudioPosition::Unit unit)
{
	mMicroseconds = ViAudioPosition::convertPosition(position, unit, ViAudioPosition::Microseconds, mFormat);
}

qreal ViAudioPosition::position(const ViAudioPosition::Unit unit) const
{
	return ViAudioPosition::convertPosition(mMicroseconds, ViAudioPosition::Microseconds, unit, mFormat);
}

QAudioFormat ViAudioPosition::format() const
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
