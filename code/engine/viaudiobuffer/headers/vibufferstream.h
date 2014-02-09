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

	protected slots:

		void inserted(qint64 position, int size);
		void removed(qint64 position, int length);

	public:

		~ViBufferStream();

		virtual int read(char *data, int length);
		virtual int read(ViBufferChunk &chunk, int length);
		virtual int read(ViBufferChunk &chunk);

		virtual int write(const char *data, int length);
		virtual int write(const ViBufferChunk &chunk, int length);
		virtual int write(const ViBufferChunk &chunk);

		// TODO: Insert doesn't use QBuffer, very slow.
		void insert(qint64 position, const char *data, int length);
		void insert(qint64 position, const ViBufferChunk &chunk, int length);
		void insert(qint64 position, const ViBufferChunk &chunk);

		void remove(qint64 position, int length);

		int size();
		void restart();
		QIODevice::OpenMode mode();

		qint64 position();
		bool setPosition(qint64 position);
		bool isValidPosition(qint64 position);
		virtual bool atEnd();
		bool hasData();
		
		ViBuffer* buffer();

	protected:

		//Protected constructor, to ensure that a QSharePointer is created from within ViBuffer
		ViBufferStream(QIODevice::OpenMode mode, ViBuffer *buffer, QByteArray *data, QMutex *mutex);

	protected:

		QIODevice::OpenMode mMode;
		ViBuffer *mBuffer;
		QBuffer *mDevice;
		QMutex *mBufferMutex;
		QMutex mStreamMutex;

};

typedef ViPointer<ViBufferStream> ViBufferStreamPointer;

#endif
