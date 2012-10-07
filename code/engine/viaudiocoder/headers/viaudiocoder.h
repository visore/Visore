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

		void convert(QString inputFilePath, QString outputFilePath, ViAudioFormat outputFormat);
		void convert(QByteArray &input, ViAudioFormat inputFormat, QByteArray &output, ViAudioFormat outputFormat);
		void convert(QString inputFilePath, QByteArray &output, ViAudioFormat outputFormat);
		void convert(QByteArray &input, ViAudioFormat inputFormat, QString outputFilePath, ViAudioFormat outputFormat);
		void convert(ViAudioBuffer *buffer, QByteArray &output, ViAudioFormat outputFormat);

		void decode(QString inputFilePath, ViAudioBuffer *buffer);
		void decode(QString inputFilePath, QByteArray &output);
		void decode(QByteArray &input, QByteArray &output);

		void encode(ViAudioBuffer *buffer, QString outputFilePath, ViAudioFormat outputFormat, ViSongInfo info = ViSongInfo());
		void encode(ViAudioBuffer *buffer, QByteArray &output, ViAudioFormat outputFormat, ViSongInfo info = ViSongInfo());
		void encode(QByteArray &input, ViAudioFormat inputFormat, QString outputFilePath, ViAudioFormat outputFormat, ViSongInfo info = ViSongInfo());
		void encode(QByteArray &input, ViAudioFormat inputFormat, QByteArray &output, ViAudioFormat outputFormat, ViSongInfo info = ViSongInfo());

		static ViCoderList coders(ViAudioManager::Mode mode = ViAudioManager::Available);
		static ViCodecList codecs(ViAudioManager::Mode mode = ViAudioManager::Available);

	private:

		ViCodingChain *mChain;
		ViCoder::Error mError;

};

#endif
