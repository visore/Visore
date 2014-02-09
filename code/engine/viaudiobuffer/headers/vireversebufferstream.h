#ifndef VIREVEREBUFFERSTREAM_H
#define VIREVEREBUFFERSTREAM_H

#include "vibufferstream.h"

typedef ViChunk<char> ViBufferChunk;

class ViBuffer;

/*
Important: Don't make ViReverseBufferStream a QObject, otherwise we can't create streams for different threads.
*/

class ViReverseBufferStream : public ViBufferStream
{

	Q_OBJECT

	friend class ViBuffer;

	public:

		int read(char *data, int length);

		void restart();

		bool atEnd();

	protected:

		//Protected constructor, to ensure that a QSharePointer is created from within ViBuffer
		ViReverseBufferStream(QIODevice::OpenMode mode, ViBuffer *buffer, QByteArray *data, QMutex *mutex);

};

typedef ViPointer<ViReverseBufferStream> ViReverseBufferStreamPointer;

#endif
