#include <viaudioinfo.h>

ViAudioInfo::ViAudioInfo()
{
	mHeaderSize = 0;
	mDataSize = 0;
	mTrailerSize = 0;
	mSize = 0;
	mSamples = 0;
}

qint32 ViAudioInfo::headerSize() const
{
	return mHeaderSize;
}

qint64 ViAudioInfo::dataSize() const
{
	return mDataSize;
}

qint32 ViAudioInfo::trailerSize() const
{
	return mTrailerSize;
}

qint64 ViAudioInfo::size() const
{
	return mSize;
}

qint64 ViAudioInfo::samples() const
{
	return mSamples;
}

void ViAudioInfo::setHeaderSize(qint32 value)
{
	mHeaderSize = value;
}

void ViAudioInfo::setDataSize(qint64 value)
{
	mDataSize = value;
}

void ViAudioInfo::setTrailerSize(qint32 value)
{
	mTrailerSize = value;
}

void ViAudioInfo::setSize(qint64 value)
{
	mSize = value;
}

void ViAudioInfo::setSamples(qint64 value)
{
	mSamples = value;
}
