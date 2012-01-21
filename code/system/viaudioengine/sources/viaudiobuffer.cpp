#include "viaudiobuffer.h"

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

ViAudioBuffer::ViAudioBuffer(QObject *parent)
	: QObject(parent)
{
	mData = new QByteArray();
	mReadStream = new QDataStream(mData, QIODevice::ReadOnly);
	mWriteStream = new QDataStream(mData, QIODevice::WriteOnly);
	mOldSize = 0;
}

ViAudioBuffer::~ViAudioBuffer()
{
	if(mReadStream != NULL)
	{
		delete mReadStream;
		mReadStream = NULL;
	}
	if(mWriteStream != NULL)
	{
		delete mWriteStream;
		mWriteStream = NULL;
	}
	if(mData != NULL)
	{
		delete mData;
		mData = NULL;
	}
}

int ViAudioBuffer::write(ViAudioBufferChunk* chunk, int length)
{
	mOldSize = mData->size();
	int written = mWriteStream->writeRawData(chunk->data(), length);
	emitChanges();
	return written;
}

int ViAudioBuffer::read(ViAudioBufferChunk* chunk, int length)
{
	char *data = new char[length];
	int read = mReadStream->readRawData(data, length);
	chunk->setData(data);
	return read;
}

int ViAudioBuffer::size()
{
	return mData->size();
}

void ViAudioBuffer::emitChanges()
{
	if(mOldSize != mData->size())
	{
		emit changed(mOldSize, mData->size()-mOldSize);
		mOldSize = mData->size();
	}
}
