#ifndef VIBUFFER_H
#define VIBUFFER_H

#include "vibufferstream.h"
#include "viaudioformat.h"
#include "vichunk.h"
#include "viid.h"

typedef ViChunk<char> ViBufferChunk;

class ViBuffer : public QObject, public ViId
{

    Q_OBJECT

	friend class ViBufferStream;

	signals:

		void changed();
		void formatChanged(ViAudioFormat format);
		void inserted(int position, int size);

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
		
		int size();
		void clear();

		ViAudioFormat format();
		ViAudioFormat& formatReference();

		int insert(int start, const char *data, int length);
		int insert(int start, const QByteArray &data);
		int insert(int start, const QByteArray &data, int length);
		int insert(int start, const ViBufferChunk &data);
		int insert(int start, const ViBufferChunk &data, int length);

	private:

		QByteArray *mData;
		ViAudioFormat mFormat;
		QMutex mMutex;

};

#endif
