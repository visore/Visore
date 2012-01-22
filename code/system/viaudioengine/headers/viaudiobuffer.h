#ifndef VIAUDIOBUFFER_H
#define VIAUDIOBUFFER_H

#include <QDataStream>
#include <QByteArray>
#include "viessentials.h"

class ViAudioBufferChunk
{
	public:
		ViAudioBufferChunk();
		ViAudioBufferChunk(char *data);
		ViAudioBufferChunk(int size);
		~ViAudioBufferChunk();
		char* data();
		void setData(char *data);
	private:
		char *mData;
};

class ViAudioBuffer : public QObject
{
    Q_OBJECT

	signals:
		//startIndex = index of the first new byte added (chunk start index), size = chunk size added to buffer
		void changed(int startIndex, int size);;

	public:
		ViAudioBuffer(QObject *parent = 0, int bufferHeadStart = 100000);
		~ViAudioBuffer();
		int write(ViAudioBufferChunk *chunk, int length);
		int read(ViAudioBufferChunk *chunk, int length);
		int size();
		void setBufferHeadStart(int bufferHeadStart);
		int bufferHeadStart();
		void clear(); //Clears all data from the stream
		void restartRead(); //Starts reading from the front of the stream again
		void restartWrite(); //Starts writing to the front of the stream again

	private:
		void emitChanges();

	private:
		QByteArray *mData;
		QDataStream *mReadStream;
		QDataStream *mWriteStream;
		qint64 mOldSize;
		int mBufferHeadStart;
		bool mHasHeadStart;
};

#endif
