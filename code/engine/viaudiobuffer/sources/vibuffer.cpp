#include "vibuffer.h"
#include <QSharedPointer>
#include "vilogger.h"
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
	for(int i = 0; i < mStreams.size(); ++i)
	{
		//mStreams.removeFirst();
		mStreams[i] = NULL;
	}
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
	pointer.setUnusedLimit(1);
	pointer.setDeleter(this);
	locker.unlock();
	addStream(pointer);
	return pointer;
}

int ViBuffer::streamCount(QIODevice::OpenMode mode)
{
	QMutexLocker locker(&mMutex);
	if(mode == QIODevice::ReadWrite)
	{
		return mStreams.size();
	}

	int counter = 0;
	for(int i = 0; i < mStreams.size(); ++i)
	{
		if(mStreams[i]->mode() == mode)
		{
			++counter;
		}
	}
	return counter;
}

void ViBuffer::addStream(ViBufferStreamPointer stream)
{
	QMutexLocker locker(&mMutex);
	mStreams.append(stream);
	locker.unlock();
	emit streamsChanged();
}

void ViBuffer::removeStream(ViBufferStream* stream)
{
	QMutexLocker locker(&mMutex);
	for(int i = 0; i < mStreams.size(); ++i)
	{
		if(stream == mStreams[i].data())
		{
			mStreams[i] = NULL;
			//mStreams.removeAt(i);LOG("removed!!!");
			break;
		}
	}
	locker.unlock();
	emit streamsChanged();
}

void ViBuffer::execute(ViFunctorParameter *data)
{
	QMutexLocker locker(&mMutex);
LOG("pppo:" + id());
	ViBufferStream *object = dynamic_cast<ViBufferStream*>(data);
	if(object != NULL)
	{
		locker.unlock();
		removeStream(object);
	}
LOG("-----------**--------- "+ QString::number(mStreams.size()));
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
