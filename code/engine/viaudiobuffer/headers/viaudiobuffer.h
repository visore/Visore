#ifndef VIAUDIOBUFFER_H
#define VIAUDIOBUFFER_H

#include "viaudiobufferstream.h"
#include "viaudioformat.h"
#include "vichunk.h"
#include "viid.h"

typedef ViChunk<char> ViAudioBufferChunk;

class ViAudioBuffer : public QObject, public ViId
{

    Q_OBJECT

	friend class ViAudioBufferStream;

	signals:

		void changed();
		void formatChanged(ViAudioFormat format);

	public slots:

		void setFormat(ViAudioFormat format);

	public:


		ViAudioBuffer();
		~ViAudioBuffer();

		QByteArray* data();
		void setData(QByteArray *data);

		ViAudioBufferStream* createReadStream();
		ViAudioBufferStream* createWriteStream();
		ViAudioBufferStream* createStream(QIODevice::OpenMode mode);
		
		int size();
		void clear();

		ViAudioFormat format();
		ViAudioFormat& formatReference();

		/*int retrieve(int start, char *data, int length);
		int retrieve(int start, QByteArray &data);
		int retrieve(int start, QByteArray &data, int length);
		int retrieve(int start, ViAudioBufferChunk &data);
		int retrieve(int start, ViAudioBufferChunk &data, int length);

		int append(const char *data, int length);
		int append(const QByteArray &data);
		int append(const QByteArray &data, int length);
		int append(const ViAudioBufferChunk &data);
		int append(const ViAudioBufferChunk &data, int length);

		int replace(int start, const char *data, int length);
		int replace(int start, const QByteArray &data);
		int replace(int start, const QByteArray &data, int length);
		int replace(int start, const ViAudioBufferChunk &data);
		int replace(int start, const ViAudioBufferChunk &data, int length);
*/
		int insert(int start, const char *data, int length);
		int insert(int start, const QByteArray &data);
		int insert(int start, const QByteArray &data, int length);
		int insert(int start, const ViAudioBufferChunk &data);
		int insert(int start, const ViAudioBufferChunk &data, int length);
/*
		int remove(int start);
		int remove(int start, int end);*/

	protected:

		int availableSize(int start, int length) const;
		bool deleteStream(ViAudioBufferStream *stream);

		void lock();
		void unlock();

	private:

		QByteArray *mData;
		ViAudioFormat mFormat;
		ViMutex mMutex;
		QList<QSharedPointer<ViAudioBufferStream> > mStreams;

};

#endif
