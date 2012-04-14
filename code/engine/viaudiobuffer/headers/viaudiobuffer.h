#ifndef VIAUDIOBUFFER_H
#define VIAUDIOBUFFER_H

#include "viaudiobufferstream.h"
#include "vichunk.h"

typedef ViChunk<char> ViAudioBufferChunk;

class ViAudioBufferStream;

class ViAudioBuffer : public QObject
{
    Q_OBJECT

	signals:
		//startIndex = index of the first new byte added (chunk start index), size = chunk size added to buffer
		void changed(int size);

	public:
		enum ViAudioBufferType
		{
			Original = 0,
			Corrected= 1
		};
		ViAudioBuffer(QObject *parent = 0, int bufferHeadStart = 100000);
		~ViAudioBuffer();
		QByteArray* data();
		void setData(QByteArray *data);
		
		int write(ViAudioBufferStream *stream, ViAudioBufferChunk *chunk, int length);
		int read(ViAudioBufferStream *stream, ViAudioBufferChunk *chunk, int length);

		ViAudioBufferStream* createWriteStream();
		ViAudioBufferStream* createReadStream();
		void deleteStream(ViAudioBufferStream* stream);
		
		int size();
		void clear(); //Clears all data from the stream
		void change(int startIndex);

	private:
		QByteArray *mData;
		QList<ViAudioBufferStream*> mStreams;
		int mBufferHeadStart;
};

#endif
