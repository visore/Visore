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
	QByteArray *data = mData;
	return data;
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
	int theSize = mData->size();
	return theSize;
}

void ViBuffer::clear()
{
	QMutexLocker locker(&mMutex);
	mData->clear();
	emit changed();
}

void ViBuffer::setFormat(ViAudioFormat format)
{
	QMutexLocker locker(&mMutex);
	mFormat = format;
	emit formatChanged(mFormat);
}

ViAudioFormat ViBuffer::format()
{
	QMutexLocker locker(&mMutex);
	ViAudioFormat format = mFormat;
	return format;
}

ViAudioFormat& ViBuffer::formatReference()
{
	QMutexLocker locker(&mMutex);
	ViAudioFormat &format = mFormat;
	return format;
}

/* Buffer access */

int ViBuffer::insert(int start, const char *data, int length)
{
	QMutexLocker locker(&mMutex);
	int newLength = mData->insert(start, data, length).size();
	emit inserted(start, length);
	emit changed();
	return newLength;
}

int ViBuffer::insert(int start, const QByteArray &data)
{
	QMutexLocker locker(&mMutex);
	int newLength = mData->insert(start, data).size();
	emit inserted(start, data.size());
	emit changed();
	return newLength;
}

int ViBuffer::insert(int start, const QByteArray &data, int length)
{
	QMutexLocker locker(&mMutex);
	int newLength = mData->insert(start, data.constData(), length).size();
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
	emit inserted(start, length);
	emit changed();
	return newLength;
}
