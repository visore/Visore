#ifndef VIAUDIOBUFFER_H
#define VIAUDIOBUFFER_H

#include <QDataStream>
#include <QByteArray>
#include <QMutex>
#include "viessentials.h"


class ViAudioBufferChunk
{
	public:
		ViAudioBufferChunk(char *data);
		~ViAudioBufferChunk();
		char* data();
	private:
		char *mData;
};

class ViAudioBuffer : public QObject
{
    Q_OBJECT

	signals:
		//startIndex = index of the first new byte added, size = new size of array
		void changed(int startIndex, int size);
		void r(ViAudioBufferChunk* b, int size);

	public:
		ViAudioBuffer(QObject *parent = 0);
		~ViAudioBuffer();
		int write(ViAudioBufferChunk* chunk, int length);
		int read(char *array, int length);
		int size();

	private:
		void emitChanges();

	private:
		QByteArray *mData;
		QDataStream *mReadStream;
		QDataStream *mWriteStream;
		int mOldSize;
QMutex mm;
bool b;
};

#endif
