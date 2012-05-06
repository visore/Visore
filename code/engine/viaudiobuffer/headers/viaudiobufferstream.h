#ifndef VIAUDIOBUFFERSTREAM_H
#define VIAUDIOBUFFERSTREAM_H

#include <QDataStream>
#include "viaudiobuffer.h"
#include "viaudiobuffermutex.h"
#include "vichunk.h"

typedef ViChunk<char> ViAudioBufferChunk;

class ViAudioBuffer;

class ViAudioBufferStream : public QObject, public QDataStream
{
	Q_OBJECT

	public:
		ViAudioBufferStream(ViAudioBuffer *buffer, QIODevice::OpenMode mode, int bufferHeadStart);

		int read(char *data, int length);
		int read(ViAudioBufferChunk *chunk, int length);
		int read(ViAudioBufferChunk *chunk);

		int write(char *data, int length);
		int write(ViAudioBufferChunk *chunk, int length);
		int write(ViAudioBufferChunk *chunk);

		void setBufferHeadStart(int bufferHeadStart);
		void setHasBufferHeadStart(bool hasHeadStart);
		int bufferHeadStart();
		int bufferSize();
		void change();
		void restart();
		bool isValidPosition(qint64 position);
		qint64 setPosition(qint64 position);
		qint64 position();
		ViAudioBuffer* buffer();

	private:
		ViAudioBufferMutex *mMutex;
		ViAudioBuffer *mBuffer;
		qint64 mOldSize;
		int mBufferHeadStart;
		bool mHasHeadStart;
};

#endif
