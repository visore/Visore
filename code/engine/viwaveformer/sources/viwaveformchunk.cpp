#include "viwaveformchunk.h"

ViWaveFormChunk::ViWaveFormChunk(qreal *data, qint64 size, ViAudioMetaData *metaData)
{
	mData = data;
	mSize = size;
	mMetaData = metaData;
	if(mMetaData == NULL)
	{
		mBytesPerSample = 0;
	}
	else
	{
		mBytesPerSample = mMetaData->bitDepth() * mMetaData->channels() / 8;
	}
}

ViWaveFormChunk::~ViWaveFormChunk()
{
	clear();
}

void ViWaveFormChunk::clear()
{
	if(mData != NULL)
	{
		delete [] mData;
		mData = NULL;
	}
	mSize = 0;
}

qreal* ViWaveFormChunk::data()
{
	return mData;
}

qint64 ViWaveFormChunk::size()
{
	return mSize;
}

qint8 ViWaveFormChunk::bytesPerSample()
{
	return mBytesPerSample;
}

ViAudioMetaData* ViWaveFormChunk::metaData()
{
	return mMetaData;
}
