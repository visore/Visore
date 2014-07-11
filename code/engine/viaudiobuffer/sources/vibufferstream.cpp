#include "vibufferstream.h"
#include "vibuffer.h"

ViBufferStream::ViBufferStream(QIODevice::OpenMode mode, ViBuffer *buffer, QByteArray *data, QMutex *mutex)
	: QObject(), ViId()
{
	mMode = mode;
	mBuffer = buffer;
	mBufferMutex = mutex;
	
	mDevice = new QBuffer(data); // We need to pass the array to the constructor, otherwise we get a deadlock, calling mBuffer->data()
	mDevice->open(mode);
}

ViBufferStream::~ViBufferStream()
{
	if(mDevice != NULL)
	{	
		mDevice->close();
		delete mDevice;
		mDevice = NULL;
	}
}

void ViBufferStream::inserted(qint64 pos, int size)
{
	int current = position();
	if(current >= pos) // Insert in a part that was already read with no overlap
	{
		setPosition(current + size);
	}
}

void ViBufferStream::removed(qint64 pos, int length)
{
	int current = position();
	if(current >= pos || pos + length > current)
	{
		if(size() > pos)
		{
			setPosition(pos);
		}
		else if(size() > 0)
		{
			setPosition(pos - 1);
		}
		else
		{
			setPosition(-1);
		}
	}
}

int ViBufferStream::read(char *data, int length)
{
	QMutexLocker streamLocker(&mStreamMutex);
	QMutexLocker bufferLocker(mBufferMutex);
	length = mDevice->read(data, length);
	return length;
}

int ViBufferStream::read(ViBufferChunk &chunk, int length)
{
	if(chunk.size() < length)
	{
		chunk.resize(length);
	}
	return read(chunk.data(), length);
}

int ViBufferStream::read(ViBufferChunk &chunk)
{
	return read(chunk.data(), chunk.size());
}

int ViBufferStream::write(const char *data, int length)
{
	QMutexLocker streamLocker(&mStreamMutex);
	QMutexLocker bufferLocker(mBufferMutex);
	int written = mDevice->write(data, length);
	bufferLocker.unlock();
	mBuffer->change();
	return written;
}

int ViBufferStream::write(const ViBufferChunk &chunk, int length)
{
	if(length > chunk.size())
	{
		length = chunk.size();
	}
	return write(chunk.data(), length);
}

int ViBufferStream::write(const ViBufferChunk &chunk)
{
	return write(chunk.data(), chunk.size());
}

void ViBufferStream::insert(qint64 position, const char *data, int length)
{
	QMutexLocker streamLocker(&mStreamMutex);
	mBuffer->insert(position, data, length);
	streamLocker.unlock();
	inserted(position, length);
}

void ViBufferStream::insert(qint64 position, const ViBufferChunk &chunk, int length)
{
	QMutexLocker streamLocker(&mStreamMutex);
	mBuffer->insert(position, chunk, length);
	streamLocker.unlock();
	inserted(position, length);
}

void ViBufferStream::insert(qint64 position, const ViBufferChunk &chunk)
{
	QMutexLocker streamLocker(&mStreamMutex);
	mBuffer->insert(position, chunk);
	streamLocker.unlock();
	inserted(position, chunk.size());
}

void ViBufferStream::remove(qint64 position, int length)
{
	QMutexLocker streamLocker(&mStreamMutex);
	if(mBuffer->remove(position, length))
	{
		streamLocker.unlock();
		removed(position, length);
	}
}

int ViBufferStream::size()
{
	QMutexLocker streamLocker(&mStreamMutex);
	return mBuffer->size();
}

void ViBufferStream::restart()
{
	setPosition(0);
}

QIODevice::OpenMode ViBufferStream::mode()
{
	return mMode;
}

qint64 ViBufferStream::position()
{
	QMutexLocker streamLocker(&mStreamMutex);
	return mDevice->pos();
}

bool ViBufferStream::setPosition(qint64 position)
{
	QMutexLocker streamLocker(&mStreamMutex);
	return mDevice->seek(position);
}

bool ViBufferStream::isValidPosition(qint64 position)
{
	return position >= 0 && position < mBuffer->size();
}

bool ViBufferStream::atEnd()
{
	QMutexLocker streamLocker(&mStreamMutex);
	return mDevice->pos() >= (mBuffer->size() - 1);
}

bool ViBufferStream::hasData()
{
	return !atEnd();
}

ViBuffer* ViBufferStream::buffer()
{
	QMutexLocker streamLocker(&mStreamMutex);
	return mBuffer;
}
