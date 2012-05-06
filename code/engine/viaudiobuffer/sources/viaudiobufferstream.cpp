#include "viaudiobufferstream.h"

#include <iostream>
using namespace std;

ViAudioBufferStream::ViAudioBufferStream(ViAudioBuffer *buffer, QIODevice::OpenMode mode, int bufferHeadStart)
	: QDataStream(buffer->data(), mode)
{
	mBuffer = buffer;
	mOldSize = 0;
	mBufferHeadStart = bufferHeadStart;
	mHasHeadStart = false;
	mMutex = ViAudioBufferMutex::instance();
}

int ViAudioBufferStream::read(char *data, int length)
{
	mMutex->lock();
	int read = readRawData(data, length);
	mMutex->unlock();
	return read;
}

int ViAudioBufferStream::read(ViAudioBufferChunk *chunk, int length)
{
	char *data = new char[length];
	int dataRead = read(data, length);
	chunk->setData(data, length);
	return dataRead;
}

int ViAudioBufferStream::read(ViAudioBufferChunk *chunk)
{
	return read(chunk, chunk->size());
}

int ViAudioBufferStream::write(char *data, int length)
{
	mMutex->lock();
	int dataWritten = writeRawData(data, length);
	mMutex->unlock();
	change();
	return dataWritten;
}

int ViAudioBufferStream::write(ViAudioBufferChunk *chunk, int length)
{
	return write(chunk->data(), length);
}

int ViAudioBufferStream::write(ViAudioBufferChunk *chunk)
{
	return write(chunk, chunk->size());
}

void ViAudioBufferStream::setBufferHeadStart(int bufferHeadStart)
{
	mBufferHeadStart = bufferHeadStart;
}

void ViAudioBufferStream::setHasBufferHeadStart(bool hasHeadStart)
{
	mHasHeadStart = hasHeadStart;
}

int ViAudioBufferStream::bufferHeadStart()
{
	return mBufferHeadStart;
}

int ViAudioBufferStream::bufferSize()
{
	mMutex->lock();
	int size = mBuffer->size();
	mMutex->unlock();
	return size;
}

void ViAudioBufferStream::change()
{
	if(mHasHeadStart && mBuffer->size() != mOldSize)
	{
		int oldSize = mBuffer->size() - mOldSize;
		mBuffer->change(oldSize);
		mOldSize = mBuffer->size();
	}
	else if(mBuffer->size() - mOldSize >= mBufferHeadStart)
	{
		int oldSize = mBuffer->size() - mOldSize;
		mBuffer->change(oldSize);
		mOldSize = mBuffer->size();
		mHasHeadStart = true;
	}	
}

void ViAudioBufferStream::restart()
{
	mOldSize = 0;
	mHasHeadStart = false;
	device()->reset();
	change();
}

bool ViAudioBufferStream::isValidPosition(qint64 position)
{
	return device()->size() >= position;
}

qint64 ViAudioBufferStream::setPosition(qint64 position)
{
	if(!isValidPosition(position))
	{
		return -1;
	}
	qint64 read = device()->seek(position);
	mOldSize = 0;
	mHasHeadStart = false;
	change();
	return read;
}

qint64 ViAudioBufferStream::position()
{
	return device()->pos();
}

ViAudioBuffer* ViAudioBufferStream::buffer()
{
	return mBuffer;
}
