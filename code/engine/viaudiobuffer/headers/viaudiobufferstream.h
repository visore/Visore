#ifndef VIAUDIOBUFFERSTREAM_H
#define VIAUDIOBUFFERSTREAM_H

#include "vimutexlocker.h"
#include "vichunk.h"
#include "viid.h"
#include <QBuffer>

typedef ViChunk<char> ViAudioBufferChunk;

class ViAudioBuffer;

/*
Important: Don't make ViAudioBufferStream a QObject, otherwise we can't create streams for different threads.
*/

class ViAudioBufferStream : public ViId
{

	friend class ViAudioBuffer;

	public:

		~ViAudioBufferStream();

		int read(char *data, int length);
		int read(ViAudioBufferChunk &chunk, int length);
		int read(ViAudioBufferChunk &chunk);

		int write(const char *data, int length);
		int write(const ViAudioBufferChunk &chunk, int length);
		int write(const ViAudioBufferChunk &chunk);

		// TODO: If we insert data, should the read position change?
		// TODO: Insert doesn't use QBuffer, very slow.
		void insert(int position, const char *data, int length);
		void insert(int position, const ViAudioBufferChunk &chunk, int length);
		void insert(int position, const ViAudioBufferChunk &chunk);

		int size();
		void restart();

		int position();
		bool setPosition(int position);
		bool isValidPosition(int position);
		bool atEnd();
		
		ViAudioBuffer* buffer();

	protected:

		//Protected constructor, to ensure that a QSharePointer is created from within ViBuffer
		ViAudioBufferStream(QIODevice::OpenMode mode, ViAudioBuffer *buffer, QByteArray *data);

	private:

		ViAudioBuffer *mBuffer;
		QBuffer *mDevice;
		ViMutex mStreamMutex;

};

#endif
