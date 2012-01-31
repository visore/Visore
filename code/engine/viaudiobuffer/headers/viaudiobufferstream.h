#ifndef VIAUDIOBUFFERSTREAM_H
#define VIAUDIOBUFFERSTREAM_H

#include <QDataStream>
#include "viaudiobufferchunk.h"
#include "viaudiobuffer.h"

class ViAudioBuffer;

class ViAudioBufferStream : public QObject, public QDataStream
{
	Q_OBJECT

	signals:
		//startIndex = index of the first new byte added (chunk start index), size = chunk size added to buffer
		void changed(int startIndex, int size);

	public:
		ViAudioBufferStream(ViAudioBuffer *buffer, QIODevice::OpenMode mode, int bufferHeadStart);
		int read(ViAudioBufferChunk *chunk, int length);
		int write(ViAudioBufferChunk *chunk, int length);
		void setBufferHeadStart(int bufferHeadStart);
		void setHasBufferHeadStart(bool hasHeadStart);
		int bufferHeadStart();
		void change();
		void restart();
		bool isValidPosition(qint64 position);
		qint64 setPosition(qint64 position);
		qint64 position();

	private:
		ViAudioBuffer *mBuffer;
		qint64 mOldSize;
		int mBufferHeadStart;
		bool mHasHeadStart;
};

#endif
