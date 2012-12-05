#ifndef VIBUFFERSTREAM_H
#define VIBUFFERSTREAM_H

#include "vichunk.h"
#include "viid.h"
#include "vipointer.h"
#include <QBuffer>
#include <QMutex>

typedef ViChunk<char> ViBufferChunk;

class ViBuffer;

/*
Important: Don't make ViBufferStream a QObject, otherwise we can't create streams for different threads.
*/

class ViBufferStream : public QObject, public ViId, public ViFunctorParameter
{

	Q_OBJECT

	friend class ViBuffer;

	private slots:

		void inserted(int position, int size);

	public:

		~ViBufferStream();

		int read(char *data, int length);
		int read(ViBufferChunk &chunk, int length);
		int read(ViBufferChunk &chunk);

		int write(const char *data, int length);
		int write(const ViBufferChunk &chunk, int length);
		int write(const ViBufferChunk &chunk);

		// TODO: Insert doesn't use QBuffer, very slow.
		void insert(int position, const char *data, int length);
		void insert(int position, const ViBufferChunk &chunk, int length);
		void insert(int position, const ViBufferChunk &chunk);

		int size();
		void restart();
		QIODevice::OpenMode mode();

		int position();
		bool setPosition(int position);
		bool isValidPosition(int position);
		bool atEnd();
		
		ViBuffer* buffer();

	protected:

		//Protected constructor, to ensure that a QSharePointer is created from within ViBuffer
		ViBufferStream(QIODevice::OpenMode mode, ViBuffer *buffer, QByteArray *data, QMutex *mutex);

	private:

		QIODevice::OpenMode mMode;
		ViBuffer *mBuffer;
		QBuffer *mDevice;
		QMutex *mBufferMutex;
		QMutex mStreamMutex;

};

typedef ViPointer<ViBufferStream> ViBufferStreamPointer;

#endif
