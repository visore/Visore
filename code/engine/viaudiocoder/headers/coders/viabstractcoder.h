#ifndef VIABSTRACTCODER_H
#define VIABSTRACTCODER_H

#include <QQueue>
#include <QMutex>
#include <QByteArray>
#include <QStringList>
#include <QLibrary>
#include <vicommon.h>
#include <viaudioformat.h>
#include <visamplearray.h>
#include <vimetadata.h>

class ViAbstractCoder : public QObject
{

	Q_OBJECT

	signals:

		void failed(ViCoder::Error error);
		void decoded(ViSampleArray *array);
		void encoded(ViSampleArray *array);
		void formatChanged(ViAudioFormat format);

	public:

		ViAbstractCoder();
		~ViAbstractCoder();

		const QList<ViAudioCodec*> supportedCodecs() const;
		virtual ViAudioCodec* detectCodec(const QByteArray &data) = 0;
		virtual QByteArray& header() = 0;
		virtual int headerSize() = 0;

		virtual bool initializeDecode() = 0;
		virtual bool finalizeDecode() = 0;
		virtual void decode(const void *input, int size) = 0;

		virtual bool initializeEncode() = 0;
		virtual bool finalizeEncode() = 0;
		virtual void encode(const void *input, int samples) = 0;
		
		virtual ViCoder::Error load();
		virtual ViCoder::Error load(QString filePath);
		virtual bool unload();

		QString filePath() const;
		void setFilePath(QString filePath);

		ViAudioFormat format(ViAudio::Mode mode) const;
		void setFormat(ViAudio::Mode mode, ViAudioFormat format);

		ViMetadata metadata() const;
		void setMetadata(const ViMetadata &metadata);

		QString name() const;
		QString version() const;
		QStringList fileNames() const;
		QStringList fileExtensions() const;
		void addFileName(QString name);
		void addFileExtension(QString extension);

		ViCoder::Error error() const;

		bool operator == (const ViAbstractCoder &other) const;
		bool operator != (const ViAbstractCoder &other) const;

	protected:

		virtual ViCoder::Error initializeLibrary() = 0;
		void setError(ViCoder::Error error);

	protected:

		QLibrary mLibrary;

		QString mName;
		QString mVersion;
		QStringList mFileNames;
		QStringList mFileExtensions;

		QByteArray mHeader;

		QList<ViAudioCodec*> mSupportedCodecs;

		ViAudioFormat mInputFormat;
		ViAudioFormat mOutputFormat;

		ViMetadata mMetadata;

	private:

		ViCoder::Error mError;

};

#endif
