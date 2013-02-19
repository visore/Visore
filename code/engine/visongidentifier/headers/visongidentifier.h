#ifndef VISONGIDENTIFIER_H
#define VISONGIDENTIFIER_H

#include <vibuffer.h>
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

		bool found();
		ViMetadata metadata();

		void setProxy(QNetworkProxy::ProxyType type, QString host, quint16 port, QString username, QString password);
		QNetworkReply::NetworkError networkError();

		void setKey(QString key);
		QString key();

		virtual void identify(ViBuffer *buffer) = 0;

	protected:

		virtual void reset();

		void finish();
		void finish(ViMetadata metadata);

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
		ViMetadata mMetadata;
		bool mFound;
		QString mKey;

};

#endif
