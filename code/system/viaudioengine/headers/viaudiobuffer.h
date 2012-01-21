#ifndef VIAUDIOBUFFER_H
#define VIAUDIOBUFFER_H

#include <QDataStream>
#include <QByteArray>
#include "viessentials.h"

/*

HSTREAM hh;
void ViBassFileInput::start()
{
QObject::connect(mBuffer, SIGNAL(changed(int , int)), this, SLOT(s(int , int)), Qt::BlockingQueuedConnection);

hh = BASS_StreamCreate(44100,2, 0, STREAMPROC_PUSH, 0);
BASS_ChannelPlay(hh, false);
}


void ViBassFileInput::s(int startIndex, int size)
{
	ViAudioBufferChunk c;
	mBuffer->read(&c, size);
	DWORD d = BASS_StreamPutData(hh, c.data(), size);
}

*/

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
		void changed(int startIndex, int size);

	public:
		ViAudioBuffer(QObject *parent = 0);
		~ViAudioBuffer();
		int write(ViAudioBufferChunk *chunk, int length);
		int read(ViAudioBufferChunk *chunk, int length);
		int size();

	private:
		void emitChanges();

	private:
		QByteArray *mData;
		QDataStream *mReadStream;
		QDataStream *mWriteStream;
		int mOldSize;

};

#endif
