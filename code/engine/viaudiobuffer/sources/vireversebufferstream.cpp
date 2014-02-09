#include "vireversebufferstream.h"
#include "vibuffer.h"

ViReverseBufferStream::ViReverseBufferStream(QIODevice::OpenMode mode, ViBuffer *buffer, QByteArray *data, QMutex *mutex)
	: ViBufferStream(mode, buffer, data, mutex)
{
	mDevice->seek(data->size() - 1);
}

int ViReverseBufferStream::read(char *data, int length)
{
	QMutexLocker streamLocker(&mStreamMutex);
	QMutexLocker bufferLocker(mBufferMutex);

	if(mDevice->pos() <= 0) return 0;
	if(mDevice->pos() < length) length = mDevice->pos();
	mDevice->seek(mDevice->pos() - length );

	char temp[length];
	length = mDevice->read(temp, length);
	for(int i = 0; i < length; ++i)
	{
		data[length - i - 1] = temp[i];
	}

	mDevice->seek(mDevice->pos() - length);
	return length;
}

void ViReverseBufferStream::restart()
{
	setPosition(mDevice->size() - 1);
}

bool ViReverseBufferStream::atEnd()
{
	QMutexLocker streamLocker(&mStreamMutex);
	return mDevice->pos() <= 0;
}
