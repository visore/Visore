#include "vibuffer.h"
#include "vilogger.h"

ViBuffer::ViBuffer()
	: QObject(), ViId()
{
	mData = NULL;
	setData(new QByteArray());

	mWriteStreamCount = 0;
	mReadStreamCount = 0;
}

ViBuffer::~ViBuffer()
{
	delete mData;
	mData = NULL;
}

QByteArray* ViBuffer::data()
{
	QMutexLocker locker(&mMutex);
	return mData;
}

void ViBuffer::setData(QByteArray *data)
{
	QMutexLocker locker(&mMutex);
	if(mData != NULL)
	{
		delete mData;
	}
	mData = data;
}

ViBufferStreamPointer ViBuffer::createReadStream(bool reversed)
{
	return createStream(QIODevice::ReadOnly, reversed);
}

ViBufferStreamPointer ViBuffer::createWriteStream(bool reversed)
{
	return createStream(QIODevice::WriteOnly, reversed);
}

ViBufferStreamPointer ViBuffer::createStream(QIODevice::OpenMode mode, bool reversed)
{
	QMutexLocker locker(&mMutex);
	if(mode == QIODevice::ReadOnly)
	{
		++mReadStreamCount;
	}
	else if(mode == QIODevice::WriteOnly)
	{
		++mWriteStreamCount;
	}
	else
	{
		LOG("Only ReadOnly and WriteOnly streams allowed");
		return ViBufferStreamPointer();
	}

	ViBufferStreamPointer pointer;
	if(reversed)
	{
		pointer = ViBufferStreamPointer(new ViReverseBufferStream(mode, this, mData, &mMutex));
	}
	else
	{
		pointer = ViBufferStreamPointer(new ViBufferStream(mode, this, mData, &mMutex));
	}
	return pointer;
}

int ViBuffer::streamCount(QIODevice::OpenMode mode)
{
	if(mode == QIODevice::ReadOnly)
	{
		return mReadStreamCount;
	}
	else if(mode == QIODevice::WriteOnly)
	{
		return mWriteStreamCount;
	}
	return -1;
}

void ViBuffer::execute(ViFunctorParameter *data)
{
	QMutexLocker locker(&mMutex);
	ViBufferStream *object = dynamic_cast<ViBufferStream*>(data);
	if(object != NULL)
	{
		if(object->mode() == QIODevice::ReadOnly)
		{
			--mReadStreamCount;
		}
		else if(object->mode() == QIODevice::WriteOnly)
		{
			--mWriteStreamCount;
			if(mWriteStreamCount == 0)
			{
				locker.unlock();
				emit unused();
			}
		}
	}
}

void ViBuffer::change()
{
	emit changed();
}

int ViBuffer::size()
{
	QMutexLocker locker(&mMutex);
	return mData->size();
}

bool ViBuffer::isEmpty()
{
	return size() == 0;
}

void ViBuffer::clear()
{
	QMutexLocker locker(&mMutex);
	mData->clear();
	locker.unlock();
	emit changed();
}

void ViBuffer::setFormat(ViAudioFormat format)
{
	QMutexLocker locker(&mMutex);
	mFormat = format;
	locker.unlock();
	emit formatChanged(mFormat);
}

ViAudioFormat ViBuffer::format()
{
	QMutexLocker locker(&mMutex);
	return mFormat;
}

ViAudioFormat& ViBuffer::formatReference()
{
	QMutexLocker locker(&mMutex);
	return mFormat;
}

/* Buffer access */

int ViBuffer::insert(int start, const char *data, int length)
{
	QMutexLocker locker(&mMutex);
	int newLength = mData->insert(start, data, length).size();
	locker.unlock();
	emit inserted(start, length);
	emit changed();
	return newLength;
}

int ViBuffer::insert(int start, const QByteArray &data)
{
	QMutexLocker locker(&mMutex);
	int newLength = mData->insert(start, data).size();
	locker.unlock();
	emit inserted(start, data.size());
	emit changed();
	return newLength;
}

int ViBuffer::insert(int start, const QByteArray &data, int length)
{
	QMutexLocker locker(&mMutex);
	int newLength = mData->insert(start, data.constData(), length).size();
	locker.unlock();
	emit inserted(start, length);
	emit changed();
	return newLength;
}

int ViBuffer::insert(int start, const ViBufferChunk &data)
{
	return insert(start, data.constData(), data.size());
}

int ViBuffer::insert(int start, const ViBufferChunk &data, int length)
{
	QMutexLocker locker(&mMutex);
	int newLength = mData->insert(start, data.constData(), length).size();
	locker.unlock();
	emit inserted(start, length);
	emit changed();
	return newLength;
}

bool ViBuffer::remove(int position, int length)
{
	QMutexLocker locker(&mMutex);
	int oldLength = mData->size();
	mData->remove(position, length);
	int newLength = oldLength - mData->size();
	locker.unlock();
	if(newLength > 0)
	{
		emit removed(position, newLength);
		return true;
	}
	return false;
}
