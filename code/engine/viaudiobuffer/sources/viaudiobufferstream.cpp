#include "viaudiobufferstream.h"
#include "viaudiobuffer.h"
#include <QMutexLocker>

ViAudioBufferStream::ViAudioBufferStream(QIODevice::OpenMode mode, ViAudioBuffer *buffer, QByteArray *data, QMutex *mutex)
	: ViId()
{
	mBuffer = buffer;
	mMutex = mutex;

	mDevice = new QBuffer(data); // We need to pass the array to the constructor, otherwise we get a deadlock, calling mBuffer->data()
	mDevice->open(mode);
}

ViAudioBufferStream::~ViAudioBufferStream()
{
	mDevice->close();
	delete mDevice;
	mBuffer->deleteStream(this);
}

int ViAudioBufferStream::read(char *data, int length)
{
	QMutexLocker locker(mMutex);
	return mDevice->read(data, length);
}

int ViAudioBufferStream::read(ViAudioBufferChunk &chunk, int length)
{
	if(chunk.size() < length)
	{
		chunk.resize(length);
	}
	return read(chunk.data(), length);
}

int ViAudioBufferStream::read(ViAudioBufferChunk &chunk)
{
	return read(chunk, chunk.size());
}

int ViAudioBufferStream::write(const char *data, int length)
{
	QMutexLocker locker(mMutex);
	int written = mDevice->write(data, length);
	emit mBuffer->changed();
	return written;
}

int ViAudioBufferStream::write(const ViAudioBufferChunk &chunk, int length)
{
	if(length > chunk.size())
	{
		length = chunk.size();
	}
	return write(chunk.data(), length);
}

int ViAudioBufferStream::write(const ViAudioBufferChunk &chunk)
{
	return write(chunk.data(), chunk.size());
}

void ViAudioBufferStream::insert(int position, const char *data, int length)
{
	mBuffer->insert(position, data, length);
}

void ViAudioBufferStream::insert(int position, const ViAudioBufferChunk &chunk, int length)
{
	mBuffer->insert(position, chunk, length);
}

void ViAudioBufferStream::insert(int position, const ViAudioBufferChunk &chunk)
{
	mBuffer->insert(position, chunk);
}

int ViAudioBufferStream::size()
{
	return mBuffer->size();
}

void ViAudioBufferStream::restart()
{
	setPosition(0);
}

int ViAudioBufferStream::position()
{
	return mDevice->pos();
}

int ViAudioBufferStream::setPosition(int position)
{
	mDevice->seek(position);
}

bool ViAudioBufferStream::isValidPosition(int position)
{
	return position < size();
}

bool ViAudioBufferStream::atEnd()
{
	return mDevice->atEnd();
}

ViAudioBuffer* ViAudioBufferStream::buffer()
{
	return mBuffer;
}
