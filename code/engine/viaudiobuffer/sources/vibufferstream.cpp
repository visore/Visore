#include "vibufferstream.h"
#include "vibuffer.h"

ViBufferStream::ViBufferStream(QIODevice::OpenMode mode, ViBuffer *buffer, QByteArray *data, QMutex *mutex)
	: QObject(), ViId()
{
	mBuffer = buffer;
	mBufferMutex = mutex;
	QObject::connect(mBuffer, SIGNAL(inserted(int, int)), this, SLOT(inserted(int, int)), Qt::DirectConnection);
	
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

void ViBufferStream::inserted(int pos, int size)
{
	int current = position();
	if(current >= pos) // Insert in a part that was already read with no overlap
	{
		setPosition(current + size);
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
	return read(chunk, chunk.size());
}

int ViBufferStream::write(const char *data, int length)
{
	QMutexLocker streamLocker(&mStreamMutex);
	QMutexLocker bufferLocker(mBufferMutex);
	int written = mDevice->write(data, length);
	emit mBuffer->changed();
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

void ViBufferStream::insert(int position, const char *data, int length)
{
	QMutexLocker streamLocker(&mStreamMutex);
	mBuffer->insert(position, data, length);
}

void ViBufferStream::insert(int position, const ViBufferChunk &chunk, int length)
{
	QMutexLocker streamLocker(&mStreamMutex);
	mBuffer->insert(position, chunk, length);
}

void ViBufferStream::insert(int position, const ViBufferChunk &chunk)
{
	QMutexLocker streamLocker(&mStreamMutex);
	mBuffer->insert(position, chunk);
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

int ViBufferStream::position()
{
	QMutexLocker streamLocker(&mStreamMutex);
	return mDevice->pos();
}

bool ViBufferStream::setPosition(int position)
{
	QMutexLocker streamLocker(&mStreamMutex);
	return mDevice->seek(position);
}

bool ViBufferStream::isValidPosition(int position)
{
	return position < mBuffer->size();
}

bool ViBufferStream::atEnd()
{
	QMutexLocker streamLocker(&mStreamMutex);
	int position = mDevice->pos();
	return position >= (mBuffer->size() - 1);
}

ViBuffer* ViBufferStream::buffer()
{
	QMutexLocker streamLocker(&mStreamMutex);
	return mBuffer;
}
