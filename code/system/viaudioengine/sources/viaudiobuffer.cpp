#include "viaudiobuffer.h"

ViAudioBufferChunk::ViAudioBufferChunk(char *data)
{
	mData = data;
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

ViAudioBuffer::ViAudioBuffer(QObject *parent)
	: QObject(parent)
{
	mData = new QByteArray();
	mReadStream = new QDataStream(mData, QIODevice::ReadOnly);
	mWriteStream = new QDataStream(mData, QIODevice::WriteOnly);
	mOldSize = 0;
b = false;
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
	//emitChanges();

emit r(chunk, length);

	return written;
}

int ViAudioBuffer::read(char *array, int length)
{

	int read = mReadStream->readRawData(array, length);

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
		emit changed(mOldSize, mData->size());
		mOldSize = mData->size();
	}
}
