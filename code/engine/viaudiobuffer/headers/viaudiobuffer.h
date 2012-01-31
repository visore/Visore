#ifndef VIAUDIOBUFFER_H
#define VIAUDIOBUFFER_H

#include "viessentials.h"
#include "viaudiobufferstream.h"

class ViAudioBufferStream;

class ViAudioBuffer : public QObject
{
    Q_OBJECT

	signals:
		//startIndex = index of the first new byte added (chunk start index), size = chunk size added to buffer
		void changed(int startIndex, int size);

	public:
		ViAudioBuffer(QObject *parent = 0, int bufferHeadStart = 100000);
		~ViAudioBuffer();
		QByteArray* data();
		
		int write(ViAudioBufferStream *stream, ViAudioBufferChunk *chunk, int length);
		int read(ViAudioBufferStream *stream, ViAudioBufferChunk *chunk, int length);
		ViAudioBufferStream* createWriteStream();
		ViAudioBufferStream* createReadStream();
		
		int size();
		void clear(); //Clears all data from the stream
		void change(int startIndex, int size);

	private:
		QByteArray *mData;
		QList<ViAudioBufferStream*> mStreams;
		int mBufferHeadStart;
};

#endif
