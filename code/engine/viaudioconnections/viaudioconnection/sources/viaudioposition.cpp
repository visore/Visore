#include "viaudioposition.h"

ViAudioPosition::ViAudioPosition(qint64 bytes, qint8 bitDepth, qint16 secondsInByte)
{
	mBytes = bytes;
	mBitDepth = bitDepth;
	if(mBitDepth <= 0)
	{
		mSample = 0;
	}
	else
	{
		mSample = mBytes / (mBitDepth / 8);
	}
	if(secondsInByte <= 0)
	{
		mSeconds = 0;
		mMilliseconds = 0;
	}
	else
	{
		mSeconds = mBytes / secondsInByte;
		mMilliseconds = mSeconds * 1000;
	}
}

qint64 ViAudioPosition::seconds()
{
	return mSeconds;
}

qint64 ViAudioPosition::milliseconds()
{
	return mMilliseconds;
}

qint64 ViAudioPosition::bytes()
{
	return mBytes;
}

qint8 ViAudioPosition::bitDepth()
{
	return mBitDepth;
}

qint64 ViAudioPosition::sample()
{
	return mSample;
}

bool ViAudioPosition::operator ==(const ViAudioPosition &other) const
{
	return mBytes == other.mBytes;
}

bool ViAudioPosition::operator !=(const ViAudioPosition &other) const
{
	return mBytes != other.mBytes;
}
