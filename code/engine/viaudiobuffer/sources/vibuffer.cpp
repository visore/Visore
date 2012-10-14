#include "vibuffer.h"
#include <QSharedPointer>

ViBuffer::ViBuffer()
	: QObject(), ViId()
{
	mData = NULL;
	setData(new QByteArray());
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

ViBufferStreamPointer ViBuffer::createReadStream()
{
	return createStream(QIODevice::ReadOnly);
}

ViBufferStreamPointer ViBuffer::createWriteStream()
{
	return createStream(QIODevice::WriteOnly);
}

ViBufferStreamPointer ViBuffer::createStream(QIODevice::OpenMode mode)
{
	QMutexLocker locker(&mMutex);
	ViBufferStreamPointer pointer = ViBufferStreamPointer(new ViBufferStream(mode, this, mData, &mMutex));
	return pointer;
}

int ViBuffer::size()
{
	QMutexLocker locker(&mMutex);
	return mData->size();
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
