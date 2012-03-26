#include "viaudiobuffer.h"

ViAudioBuffer::ViAudioBuffer(QObject *parent, int bufferHeadStart)
	: QObject(parent)
{
	mData = new QByteArray();
	mBufferHeadStart = bufferHeadStart;
}

ViAudioBuffer::~ViAudioBuffer()
{
	for(int i = 0; i < mStreams.size(); ++i)
	{	
		if(mStreams[i] != NULL)
		{
			delete mStreams[i];
			mStreams[i] = NULL;
		}
	}
	if(mData != NULL)
	{
		delete mData;
		mData = NULL;
	}
}

QByteArray* ViAudioBuffer::data()
{
	return mData;
}

int ViAudioBuffer::write(ViAudioBufferStream *stream, ViAudioBufferChunk *chunk, int length, int id)
{
	return stream->write(chunk, length, id);
}

int ViAudioBuffer::read(ViAudioBufferStream *stream, ViAudioBufferChunk *chunk, int length)
{
	return stream->read(chunk, length);
}

ViAudioBufferStream* ViAudioBuffer::createWriteStream()
{
	ViAudioBufferStream *stream = new ViAudioBufferStream(this, QIODevice::WriteOnly, mBufferHeadStart);
	mStreams.append(stream);
	return stream;
}

ViAudioBufferStream* ViAudioBuffer::createReadStream()
{
	ViAudioBufferStream *stream = new ViAudioBufferStream(this, QIODevice::ReadOnly, mBufferHeadStart);
	mStreams.append(stream);
	return stream;
}

void ViAudioBuffer::deleteStream(ViAudioBufferStream* stream)
{
	for(int i = 0; i < mStreams.size(); ++i)
	{
		if(stream == mStreams[i])
		{
			delete mStreams[i];
			mStreams[i] = NULL;
			mStreams.removeAt(i);
			return;
		}
	}
}

int ViAudioBuffer::size()
{
	if(mData == NULL)
	{
		return 0;
	}
	return mData->size();
}

void ViAudioBuffer::clear()
{
	mData->clear();
	for(int i = 0; i < mStreams.size(); ++i)
	{	
		if(mStreams[i] != NULL)
		{
			mStreams[i]->restart();
		}
	}
}

void ViAudioBuffer::change(int size, int id)
{
	emit changed(size, id);
}
