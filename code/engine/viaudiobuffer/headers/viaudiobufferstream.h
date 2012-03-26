#ifndef VIAUDIOBUFFERSTREAM_H
#define VIAUDIOBUFFERSTREAM_H

#include <QDataStream>
#include "viaudiobufferchunk.h"
#include "viaudiobuffer.h"
#include "viaudiobuffermutex.h"

class ViAudioBuffer;

class ViAudioBufferStream : public QObject, public QDataStream
{
	Q_OBJECT

	public:
		ViAudioBufferStream(ViAudioBuffer *buffer, QIODevice::OpenMode mode, int bufferHeadStart);
		int read(ViAudioBufferChunk *chunk, int length);
		int write(ViAudioBufferChunk *chunk, int length, int id = -1);
		void setBufferHeadStart(int bufferHeadStart);
		void setHasBufferHeadStart(bool hasHeadStart);
		int bufferHeadStart();
		int bufferSize();
		void change(int id = -1);
		void restart();
		bool isValidPosition(qint64 position);
		qint64 setPosition(qint64 position);
		qint64 position();

	private:
		ViAudioBufferMutex *mMutex;
		ViAudioBuffer *mBuffer;
		qint64 mOldSize;
		int mBufferHeadStart;
		bool mHasHeadStart;
};

#endif
