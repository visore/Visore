#include "viaudiobufferstream.h"
#include "viaudiobuffer.h"

ViAudioBufferStream::ViAudioBufferStream(QIODevice::OpenMode mode, ViAudioBuffer *buffer, QByteArray *data)
	: ViId()
{
	mBuffer = buffer;
	//mMutex = mutex;
	
	mDevice = new QBuffer(data); // We need to pass the array to the constructor, otherwise we get a deadlock, calling mBuffer->data()
	mDevice->open(mode);

	//cout<<"Mutex: " << &mStreamMutex <<" "<<this<<" "<<mBuffer<< endl;
}

ViAudioBufferStream::~ViAudioBufferStream()
{
	if(mDevice != NULL)
	{	
		mDevice->close();
		delete mDevice;
		mDevice = NULL;
	}
	mBuffer->deleteStream(this);
}

int ViAudioBufferStream::read(char *data, int length)
{
	QMutexLocker streamLocker(&mStreamMutex);
	mBuffer->lock();
	length = mDevice->read(data, length);
	mBuffer->unlock();
	return length;
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
	QMutexLocker streamLocker(&mStreamMutex);
	mBuffer->lock();
	int written = mDevice->write(data, length);
	mBuffer->unlock();
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
	QMutexLocker streamLocker(&mStreamMutex);
	mBuffer->insert(position, data, length);
}

void ViAudioBufferStream::insert(int position, const ViAudioBufferChunk &chunk, int length)
{
	QMutexLocker streamLocker(&mStreamMutex);
	mBuffer->insert(position, chunk, length);
}

void ViAudioBufferStream::insert(int position, const ViAudioBufferChunk &chunk)
{
	QMutexLocker streamLocker(&mStreamMutex);
	mBuffer->insert(position, chunk);
}

int ViAudioBufferStream::size()
{
	//cout<<"Mutex0: " << &mStreamMutex <<" "<<this<<" "<<mBuffer<< endl;
	QMutexLocker streamLocker(&mStreamMutex);
	return mBuffer->size();
}

void ViAudioBufferStream::restart()
{
	setPosition(0);
}

int ViAudioBufferStream::position()
{
	//cout<<"Mutex1: " << &mStreamMutex <<" "<<this<<" "<<mBuffer<< endl;
	QMutexLocker streamLocker(&mStreamMutex);
	return mDevice->pos();
}

bool ViAudioBufferStream::setPosition(int position)
{
	//cout<<"Mutex2: " << &mStreamMutex <<" "<<this<<" "<<mBuffer<< endl;
	QMutexLocker streamLocker(&mStreamMutex);
	return mDevice->seek(position);
}

bool ViAudioBufferStream::isValidPosition(int position)
{
	int theSize = size();
	QMutexLocker streamLocker(&mStreamMutex);
	return position < theSize;
}

bool ViAudioBufferStream::atEnd()
{
	return position() >= (size() - 1);
}

ViAudioBuffer* ViAudioBufferStream::buffer()
{
	//cout<<"Mutex99: " << &mStreamMutex <<" "<<this<<" "<<mBuffer<< endl;
	QMutexLocker streamLocker(&mStreamMutex);
	return mBuffer;
}
