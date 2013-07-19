#ifndef VIBUFFER_H
#define VIBUFFER_H

#include "vibufferstream.h"
#include "viaudioformat.h"
#include "vichunk.h"
#include "viid.h"

typedef ViRawChunk ViBufferChunk;

class ViBuffer : public QObject, public ViId, public ViFunctor
{

    Q_OBJECT

	friend class ViBufferStream;

	signals:

		void changed();
		void unused();
		void formatChanged(ViAudioFormat format);
		void inserted(int position, int size);
		void removed(int position, int length);

	public slots:

		void setFormat(ViAudioFormat format);

	public:


		ViBuffer();
		~ViBuffer();

		QByteArray* data();
		void setData(QByteArray *data);

		ViBufferStreamPointer createReadStream();
		ViBufferStreamPointer createWriteStream();
		ViBufferStreamPointer createStream(QIODevice::OpenMode mode);
		int streamCount(QIODevice::OpenMode mode = QIODevice::ReadWrite);
		
		int size();
		void clear();
		bool isEmpty();

		ViAudioFormat format();
		ViAudioFormat& formatReference();

		int insert(int start, const char *data, int length);
		int insert(int start, const QByteArray &data);
		int insert(int start, const QByteArray &data, int length);
		int insert(int start, const ViBufferChunk &data);
		int insert(int start, const ViBufferChunk &data, int length);

		bool remove(int position, int length);

	protected:

		void execute(ViFunctorParameter *data = NULL);
		void change();

	private:

		QByteArray *mData;
		ViAudioFormat mFormat;
		QMutex mMutex;
		
		int mWriteStreamCount;
		int mReadStreamCount;
		QList<ViBufferStreamPointer> mStreams;

};

#endif
