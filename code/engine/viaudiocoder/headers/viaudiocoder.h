#ifndef VIAUDIOCODER_H
#define VIAUDIOCODER_H

#include <vicodingchain.h>
#include <viaudiomanager.h>
#include <visonginfo.h>

class ViAudioCoder : public QObject
{

	Q_OBJECT

	signals:

		void failed(ViCoder::Error error);
		void progressed(qreal percentage);
		void finished();
		void formatChanged(ViAudioFormat format);

	private slots:

		void setError(ViCoder::Error error);

	public:

		ViAudioCoder();
		~ViAudioCoder();
		ViCoder::Error error();
		QString errorString();
		bool isRunning();
		bool isFinished();
		void stop();

		void addSearchPath(QString path);
		void addFileName(QString coder, QString name);
		void addFileExtension(QString coder, QString extension);

		void convert(QString inputFilePath, QString outputFilePath, ViAudioFormat outputFormat, qint64 fromOffset = -1, qint64 toOffset = -1);
		void convert(QByteArray &input, ViAudioFormat inputFormat, QByteArray &output, ViAudioFormat outputFormat, qint64 fromOffset = -1, qint64 toOffset = -1);
		void convert(QString inputFilePath, QByteArray &output, ViAudioFormat outputFormat, qint64 fromOffset = -1, qint64 toOffset = -1);
		void convert(QByteArray &input, ViAudioFormat inputFormat, QString outputFilePath, ViAudioFormat outputFormat, qint64 fromOffset = -1, qint64 toOffset = -1);
		void convert(ViBuffer *buffer, QByteArray &output, ViAudioFormat outputFormat, qint64 fromOffset = -1, qint64 toOffset = -1);
		void convert(ViBuffer *buffer, ViBuffer *output, ViAudioFormat outputFormat, qint64 fromOffset = -1, qint64 toOffset = -1);

		void decode(QString inputFilePath, ViBuffer *buffer, qint64 fromOffset = -1, qint64 toOffset = -1);
		void decode(QString inputFilePath, QByteArray &output, qint64 fromOffset = -1, qint64 toOffset = -1);
		void decode(QByteArray &input, QByteArray &output, qint64 fromOffset = -1, qint64 toOffset = -1);

		void encode(ViBuffer *buffer, QString outputFilePath, ViAudioFormat outputFormat, ViSongInfo info = ViSongInfo(), qint64 fromOffset = -1, qint64 toOffset = -1);
		void encode(ViBuffer *buffer, QByteArray &output, ViAudioFormat outputFormat, ViSongInfo info = ViSongInfo(), qint64 fromOffset = -1, qint64 toOffset = -1);
		void encode(ViBuffer *buffer, ViBuffer *output, ViAudioFormat outputFormat, ViSongInfo info = ViSongInfo(), qint64 fromOffset = -1, qint64 toOffset = -1);
		void encode(QByteArray &input, ViAudioFormat inputFormat, QString outputFilePath, ViAudioFormat outputFormat, ViSongInfo info = ViSongInfo(), qint64 fromOffset = -1, qint64 toOffset = -1);
		void encode(QByteArray &input, ViAudioFormat inputFormat, QByteArray &output, ViAudioFormat outputFormat, ViSongInfo info = ViSongInfo(), qint64 fromOffset = -1, qint64 toOffset = -1);

		static ViCoderList coders(ViAudioManager::Mode mode = ViAudioManager::Available);
		static ViCodecList codecs(ViAudioManager::Mode mode = ViAudioManager::Available);

	private:

		ViCodingChain *mChain;
		ViCoder::Error mError;

};

#endif
