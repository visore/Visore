#include "viaudioposition.h"

ViAudioPosition::ViAudioPosition()
{
	mSamples = 0;
	mFormat = ViAudioFormat();
}

ViAudioPosition::ViAudioPosition(const ViAudioPosition &other)
{
	mSamples = other.mSamples;
	mFormat = other.mFormat;
}

ViAudioPosition::ViAudioPosition(const qreal value, const ViAudioPosition::Unit unit, const ViAudioFormat format)
{
	setPosition(value, unit, format);
}

qreal ViAudioPosition::convertPosition(const qreal position, const ViAudioPosition::Unit fromUnit, const ViAudioPosition::Unit toUnit, const ViAudioFormat &format)
{
	qreal samples = 0;

	if(fromUnit == ViAudioPosition::Microseconds)
	{
		samples = (position / 1000000.0) * format.sampleRate() * format.channelCount();
	}
	else if(fromUnit == ViAudioPosition::Milliseconds)
	{
		samples = (position / 1000.0) * format.sampleRate() * format.channelCount();
	}
	else if(fromUnit == ViAudioPosition::Seconds)
	{
		samples = position * format.sampleRate() * format.channelCount();
	}
	else if(fromUnit == ViAudioPosition::Samples)
	{
		samples = position;
	}
	else if(fromUnit == ViAudioPosition::Bytes)
	{
		samples = position / (format.sampleSize() / 8.0);
	}

	if(toUnit == ViAudioPosition::Microseconds)
	{
		return (samples * 1000000.0) / (format.sampleRate() * format.channelCount());
	}
	else if(toUnit == ViAudioPosition::Milliseconds)
	{
		return (samples * 1000.0) / (format.sampleRate() * format.channelCount());
	}
	else if(toUnit == ViAudioPosition::Seconds)
	{
		return samples / float(format.sampleRate() * format.channelCount());
	}
	else if(toUnit == ViAudioPosition::Samples)
	{
		return samples;
	}
	else if(toUnit == ViAudioPosition::Bytes)
	{
		return samples * (format.sampleSize() / 8.0);
	}
}

void ViAudioPosition::setPosition(const qreal position, const ViAudioPosition::Unit unit, const ViAudioFormat format)
{
	mFormat = format;
	setPosition(position, unit);
}

void ViAudioPosition::setPosition(const qreal position, const ViAudioPosition::Unit unit)
{
	mSamples = ViAudioPosition::convertPosition(position, unit, ViAudioPosition::Samples, mFormat);
}

qreal ViAudioPosition::position(const ViAudioPosition::Unit unit) const
{
	return ViAudioPosition::convertPosition(mSamples, ViAudioPosition::Samples, unit, mFormat);
}

void ViAudioPosition::setMicroseconds(qreal value)
{
	setPosition(value, ViAudioPosition::Microseconds);
}

void ViAudioPosition::setMilliseconds(qreal value)
{
	setPosition(value, ViAudioPosition::Milliseconds);
}

void ViAudioPosition::setSeconds(qreal value)
{
	setPosition(value, ViAudioPosition::Seconds);
}

void ViAudioPosition::setSamples(qreal value)
{
	setPosition(value, ViAudioPosition::Samples);
}

void ViAudioPosition::setBytes(qreal value)
{
	setPosition(value, ViAudioPosition::Bytes);
}

qreal ViAudioPosition::microseconds()
{
	return position(ViAudioPosition::Microseconds);
}

qreal ViAudioPosition::milliseconds()
{
	return position(ViAudioPosition::Milliseconds);
}

qreal ViAudioPosition::seconds()
{
	return position(ViAudioPosition::Seconds);
}

qreal ViAudioPosition::samples()
{
	return position(ViAudioPosition::Samples);
}

qreal ViAudioPosition::bytes()
{
	return position(ViAudioPosition::Bytes);
}

ViAudioFormat ViAudioPosition::format() const
{
	return mFormat;
}

bool ViAudioPosition::isValid() const
{
	return mFormat.isValid() && mSamples >= 0;
}

bool ViAudioPosition::operator ==(const ViAudioPosition &other) const
{
	return mSamples == other.mSamples;
}

bool ViAudioPosition::operator !=(const ViAudioPosition &other) const
{
	return mSamples != other.mSamples;
}
