#include "viaudiobufferchunk.h"

ViAudioBufferChunk::ViAudioBufferChunk()
{
	mData = NULL;
}

ViAudioBufferChunk::ViAudioBufferChunk(char *data)
{
	mData = data;
}

ViAudioBufferChunk::ViAudioBufferChunk(int size)
{
	mData = new char[size];
}

ViAudioBufferChunk::~ViAudioBufferChunk()
{
	if(mData != NULL)
	{
		delete [] mData;
		mData = NULL;
	}
}

char* ViAudioBufferChunk::data()
{
	return mData;
}

void ViAudioBufferChunk::setData(char *data)
{
	if(mData != NULL)
	{
		delete [] mData;
	}
	mData = data;
}
