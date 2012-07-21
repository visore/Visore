#ifndef VIAUDIOCODER_H
#define VIAUDIOCODER_H

#include <vicodingchain.h>
#include <viaudiomanager.h>

class ViAudioCoder : public QObject
{

	Q_OBJECT

	signals:

		void failed(ViCoder::Error error);
		void progressed(qreal percentage);
		void finished();

	private slots:

		void setError(ViCoder::Error error);

	public:

		ViAudioCoder();
		~ViAudioCoder();
		ViCoder::Error error();
		QString errorString();

		void addSearchPath(QString path);
		void addFileName(QString coder, QString name);
		void addFileExtension(QString coder, QString extension);

		void convert(QString inputFilePath, QString outputFilePath, ViAudioFormat outputFormat);
		void convert(QByteArray &input, ViAudioFormat inputFormat, QByteArray &output, ViAudioFormat outputFormat);
		void convert(QString inputFilePath, QByteArray &output, ViAudioFormat outputFormat);
		void convert(QByteArray &input, ViAudioFormat inputFormat, QString outputFilePath, ViAudioFormat outputFormat);

		void decode(QString inputFilePath, QByteArray &output, ViAudioFormat &inputFormat);
		void decode(QByteArray &input, QByteArray &output, ViAudioFormat &inputFormat);

		void encode(QByteArray &input, ViAudioFormat inputFormat, QString outputFilePath, ViAudioFormat outputFormat);
		void encode(QByteArray &input, ViAudioFormat inputFormat, QByteArray &output, ViAudioFormat outputFormat);

		static ViCoderList coders(ViAudioManager::Mode mode = ViAudioManager::Available);
		static ViCodecList codecs(ViAudioManager::Mode mode = ViAudioManager::Available);

	private:

		ViCodingChain *mChain;
		ViCoder::Error mError;

};

#endif
