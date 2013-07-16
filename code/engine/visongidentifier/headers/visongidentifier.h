#ifndef VISONGIDENTIFIER_H
#define VISONGIDENTIFIER_H

#include <vifingerprinter.h>
#include <vimetadata.h>
#include <viwebservicer.h>

class ViSongIdentifier : public QObject
{

	Q_OBJECT

	signals:

		void identified(bool success);

	protected slots:

		virtual void processReply(bool success);

	public:

		ViSongIdentifier();
		virtual ~ViSongIdentifier();

		static ViMetadata metadata(QList<ViSongIdentifier*> identifiers);
		static ViMetadata metadata(QList<ViMetadata> metadatas);

		bool found();
		QList<ViMetadata> metadatas();

		void setProxy(QNetworkProxy::ProxyType type, QString host, quint16 port, QString username, QString password);
		QNetworkReply::NetworkError networkError();

		void setKey(QString key);
		QString key();

		void identify(ViBufferOffsets bufferOffset);
		virtual void identifyTrack(ViBufferOffsets bufferOffset) = 0;

		virtual void reset();

	protected:

		void finish();
		void finish(QList<ViMetadata> metadatas);

		void retrieve(QString url);
		void retrieve(QString url, QJsonObject jsonObject);
		void retrieve(QString url, ViWebParameters parameters);
		QString saveImage(QByteArray &data, QString name);

		void redirectReply(const char *slot);

		QString stringResult();
		QJsonObject jsonResult();
		QByteArray& byteResult();
		QString url();

	private:

		ViWebServicer mServicer;
		QList<ViMetadata> mMetadatas;
		bool mFound;
		QString mKey;

};

#endif
