#include "vireversebufferstream.h"
#include "vibuffer.h"

#define OFFSET 4 // (number of channels * number of bytes per sample) . For stereo 16 bit samples = (2 * 2) = 4

ViReverseBufferStream::ViReverseBufferStream(QIODevice::OpenMode mode, ViBuffer *buffer, QByteArray *data, QMutex *mutex)
	: ViBufferStream(mode, buffer, data, mutex)
{
	if(mDevice->size() - OFFSET >= 0) mDevice->seek(mDevice->size() - OFFSET);
}

int ViReverseBufferStream::read(char *data, int length)
{
	// TODO: This function currently only works with 16 bit samples

	QMutexLocker streamLocker(&mStreamMutex);
	QMutexLocker bufferLocker(mBufferMutex);

	if(mDevice->pos() <= 0) return 0;
	if(mDevice->pos() < length - OFFSET) length = mDevice->pos() + OFFSET;

	mDevice->seek(mDevice->pos() - length + OFFSET);

	char temp[length];
	length = mDevice->peek(temp, length);

	qint16 *tempSamples = reinterpret_cast<qint16*>(temp); // Cast to sample size
	qint16* dataSamples = reinterpret_cast<qint16*>(data);
	int end = length / 2; // devide by bytes per sample
	for(int i = 0; i < end; ++i) dataSamples[end - i - 1] = tempSamples[i];

	if(mDevice->pos() - OFFSET >= 0) mDevice->seek(mDevice->pos() - OFFSET);
	return length;
}

void ViReverseBufferStream::restart()
{
	QMutexLocker streamLocker(&mStreamMutex);
	if(mDevice->size() - OFFSET >= 0) mDevice->seek(mDevice->size() - OFFSET);
}

bool ViReverseBufferStream::atEnd()
{
	QMutexLocker streamLocker(&mStreamMutex);
	return mDevice->pos() <= 0;
}
