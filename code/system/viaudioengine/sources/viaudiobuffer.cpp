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

ViAudioBuffer::ViAudioBuffer(QObject *parent, int bufferHeadStart)
	: QObject(parent)
{
	mData = new QByteArray();
	mReadStream = new QDataStream(mData, QIODevice::ReadOnly);
	mWriteStream = new QDataStream(mData, QIODevice::WriteOnly);
	mOldSize = 0;
	mBufferHeadStart = bufferHeadStart;
	mHasHeadStart = false;
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
	if(mData == NULL)
	{
		return 0;
	}
	return mData->size();
}

void ViAudioBuffer::setBufferHeadStart(int bufferHeadStart)
{
	mBufferHeadStart = bufferHeadStart;
}

int ViAudioBuffer::bufferHeadStart()
{
	return mBufferHeadStart;
}

void ViAudioBuffer::emitChanges()
{
	if(mHasHeadStart)
	{
		if(mData->size() != mOldSize)
		{
			emit changed(mOldSize, mData->size() - mOldSize);
			mOldSize = mData->size();
		}
	}
	else
	{
		if((mData->size() - mOldSize) >= mBufferHeadStart)
		{
			emit changed(mOldSize, mData->size() - mOldSize);
			mOldSize = mData->size();
			mHasHeadStart = true;
		}
	}
}

void ViAudioBuffer::clear()
{
	if(mData != NULL)
	{
		delete mData;
		mData = NULL;
	}
	restartRead();
	restartWrite();
}

void ViAudioBuffer::restartRead()
{
	if(mReadStream != NULL)
	{
		delete mReadStream;
		mReadStream = NULL;
	}
	mOldSize = 0;
	mHasHeadStart = false;
	mReadStream = new QDataStream(mData, QIODevice::ReadOnly);
	emitChanges();
}

void ViAudioBuffer::restartWrite()
{
	if(mWriteStream != NULL)
	{
		delete mWriteStream;
		mWriteStream = NULL;
	}
	mOldSize = 0;
	mHasHeadStart = false;
	mWriteStream = new QDataStream(mData, QIODevice::WriteOnly);
}
