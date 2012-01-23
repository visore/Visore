#include "viaudiometadata.h"

void ViAudioMetaData::setFormat(ViAudioFormat format)
{
	mFormat = format;
}

void ViAudioMetaData::setFrequency(int frequency)
{
	mFrequency = frequency;
}

void ViAudioMetaData::setChannels(int channels)
{
	mChannels = channels;
}

void ViAudioMetaData::setBytes(qint64 bytes)
{
	mBytes = bytes;
}

void ViAudioMetaData::setMilliseconds(qint64 milliseconds)
{
	mMilliseconds = milliseconds;
}

void ViAudioMetaData::setSeconds(qint64 seconds)
{
	mMilliseconds = seconds * 1000;
}

ViAudioFormat ViAudioMetaData::format()
{
	return mFormat;
}

int ViAudioMetaData::frequency()
{
	return mFrequency;
}

int ViAudioMetaData::channels()
{
	return mChannels;
}

qint64 ViAudioMetaData::bytes()
{
	return mBytes;
}

qint64 ViAudioMetaData::milliseconds()
{
	return mMilliseconds;
}

qint64 ViAudioMetaData::seconds()
{
	return mMilliseconds / 1000;
}
