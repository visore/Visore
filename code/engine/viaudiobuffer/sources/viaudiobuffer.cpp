#include "viaudiobuffer.h"
#include <QMutexLocker>
#include <QSharedPointer>
#include "vilogger.h"
ViAudioBuffer::ViAudioBuffer()
	: QObject(), ViId()
{
	mData = NULL;
	setData(new QByteArray());
}

ViAudioBuffer::~ViAudioBuffer()
{
	mStreams.clear();
	delete mData;
	mData = NULL;
}

QByteArray* ViAudioBuffer::data()
{
	QMutexLocker locker(&mMutex);
	return mData;
}

void ViAudioBuffer::setData(QByteArray *data)
{
	QMutexLocker locker(&mMutex);
	if(mData != NULL)
	{
		delete mData;
	}
	mData = data;
}

ViAudioBufferStream* ViAudioBuffer::createReadStream()
{
	return createStream(QIODevice::ReadOnly);
}

ViAudioBufferStream* ViAudioBuffer::createWriteStream()
{
	return createStream(QIODevice::WriteOnly);
}

ViAudioBufferStream* ViAudioBuffer::createStream(QIODevice::OpenMode mode)
{
	QMutexLocker locker(&mMutex);
	QSharedPointer<ViAudioBufferStream> pointer = QSharedPointer<ViAudioBufferStream>(new ViAudioBufferStream(mode, this, mData, &mMutex));
	mStreams.append(pointer);
	return pointer.data();
}

int ViAudioBuffer::size()
{
	QMutexLocker locker(&mMutex);
	return mData->size();
}

void ViAudioBuffer::clear()
{
	QMutexLocker locker(&mMutex);
	mData->clear();
	emit changed();
}

void ViAudioBuffer::setFormat(ViAudioFormat format)
{LOG("***uuu: 0");
	QMutexLocker locker(&mMutex);LOG("***uuu: 1");
	mFormat = format;LOG("***uuu: 2");
	locker.unlock();
	emit formatChanged(mFormat);LOG("***uuu: 3");
}

ViAudioFormat ViAudioBuffer::format()
{
	QMutexLocker locker(&mMutex);
	return mFormat;
}

ViAudioFormat& ViAudioBuffer::formatReference()
{
	QMutexLocker locker(&mMutex);
	return mFormat;
}

bool ViAudioBuffer::deleteStream(ViAudioBufferStream *stream)
{
	for(int i = 0; i < mStreams.size(); ++i)
	{
		if(*stream == *mStreams[i].data()) // Compare IDs
		{
			mStreams.takeAt(i); // Do not removeAt, since it will call the desctructor again
			return true;
		}
	}
	return false;
}

/* Buffer access */

int ViAudioBuffer::availableSize(int start, int length) const
{
	start = mData->size() - start;
	if(start < length)
	{
		return start;
	}
	return length;
}

int ViAudioBuffer::retrieve(int start, char *data, int length)
{
	QMutexLocker locker(&mMutex);
	length = availableSize(start, length);
	memcpy(data, mData->constData(), length);
	return length;
}

int ViAudioBuffer::retrieve(int start, QByteArray &data)
{
	return retrieve(start, data, data.size());
}

int ViAudioBuffer::retrieve(int start, QByteArray &data, int length)
{
	QMutexLocker locker(&mMutex);
	length = availableSize(start, length);
	if(length != data.size())
	{
		data.resize(length);
	}
	memcpy(data.data(), mData->constData(), length);
	return length;
}

int ViAudioBuffer::retrieve(int start, ViAudioBufferChunk &data)
{
	return retrieve(start, data.data(), data.size());
}

int ViAudioBuffer::retrieve(int start, ViAudioBufferChunk &data, int length)
{
	QMutexLocker locker(&mMutex);
	length = availableSize(start, length);
	if(length != data.size())
	{
		data.resize(length);
	}
	memcpy(data.data(), mData->constData(), length);
	return length;
}

int ViAudioBuffer::append(const char *data, int length)
{
	QMutexLocker locker(&mMutex);
	length = mData->append(data, length).size();
	emit changed();
	return length;
}

int ViAudioBuffer::append(const QByteArray &data)
{
	QMutexLocker locker(&mMutex);
	int newSize = mData->append(data).size();
	emit changed();
	return newSize;
}

int ViAudioBuffer::append(const QByteArray &data, int length)
{
	QMutexLocker locker(&mMutex);
	if(data.size() < length)
	{
		length = data.size();
	}
	length = mData->append(data.constData(), length).size();
	emit changed();
	return length;
}

int ViAudioBuffer::append(const ViAudioBufferChunk &data)
{
	QMutexLocker locker(&mMutex);
	int newSize = mData->append(data.constData(), data.size()).size();
	emit changed();
	return newSize;
}

int ViAudioBuffer::append(const ViAudioBufferChunk &data, int length)
{
	QMutexLocker locker(&mMutex);
	if(data.size() < length)
	{
		length = data.size();
	}
	length = mData->append(data.constData(), length).size();
	emit changed();
	return length;
}

int ViAudioBuffer::replace(int start, const char *data, int length)
{
	QMutexLocker locker(&mMutex);
	length = mData->replace(start, length, data).size();
	emit changed();
	return length;
}

int ViAudioBuffer::replace(int start, const QByteArray &data)
{
	return replace(start, data.constData(), data.size());
}

int ViAudioBuffer::replace(int start, const QByteArray &data, int length)
{
	QMutexLocker locker(&mMutex);
	if(data.size() < length)
	{
		length = data.size();
	}
	length = mData->replace(start, length, data).size();
	emit changed();
	return length;
}

int ViAudioBuffer::replace(int start, const ViAudioBufferChunk &data)
{
	return replace(start, data.constData(), data.size());
}

int ViAudioBuffer::replace(int start, const ViAudioBufferChunk &data, int length)
{
	QMutexLocker locker(&mMutex);
	if(data.size() < length)
	{
		length = data.size();
	}
	length = mData->replace(start, length, data.constData()).size();
	emit changed();
	return length;
}

int ViAudioBuffer::insert(int start, const char *data, int length)
{
	QMutexLocker locker(&mMutex);
	length = mData->insert(start, data, length).size();
	emit changed();
	return length;
}

int ViAudioBuffer::insert(int start, const QByteArray &data)
{
	QMutexLocker locker(&mMutex);
	int newSize = mData->insert(start, data).size();
	emit changed();
	return newSize;
}

int ViAudioBuffer::insert(int start, const QByteArray &data, int length)
{
	QMutexLocker locker(&mMutex);
	if(data.size() < length)
	{
		length = data.size();
	}
	length = mData->insert(start, data.constData(), length).size();
	emit changed();
	return length;
}

int ViAudioBuffer::insert(int start, const ViAudioBufferChunk &data)
{
	return insert(start, data.constData(), data.size());
}

int ViAudioBuffer::insert(int start, const ViAudioBufferChunk &data, int length)
{
	QMutexLocker locker(&mMutex);
	if(data.size() < length)
	{
		length = data.size();
	}
	length = mData->insert(start, data.constData(), length).size();
	emit changed();
	return length;
}

int ViAudioBuffer::remove(int start)
{
	return remove(start, mData->size() - start);
}

int ViAudioBuffer::remove(int start, int end)
{
	QMutexLocker locker(&mMutex);
	end = mData->remove(start, end).size();
	emit changed();
	return end;
}
