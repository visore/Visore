#ifndef VIWEBSERVICER_H
#define VIWEBSERVICER_H

#include <QUrl>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QNetworkProxy>
#include <QNetworkReply>

class ViWebParameter
{

	public:

		ViWebParameter();
		ViWebParameter(QString name, QString value);
		ViWebParameter(const ViWebParameter &other);
		void setName(QString name);
		void setValue(QString value);
		QString name();
		QString value();

	private:

		QString mName;
		QString mValue;

};

class ViWebParameters : public QList<ViWebParameter>
{

	public:

		ViWebParameters();
		void append(QString name, QString value);

};

class ViWebServicer : public QObject
{
	Q_OBJECT

	signals:

		void finished(bool success);

	private slots:

		void processReply(QNetworkReply *reply);

	public:

		ViWebServicer();

		void setProxy(QNetworkProxy::ProxyType type, QString host, quint16 port, QString username, QString password);

		void retrieve(QUrl url);
		void retrieve(QString url);
		void retrieve(QUrl url, QJsonObject jsonObject);
		void retrieve(QString url, QJsonObject jsonObject);
		void retrieve(QUrl url, ViWebParameters parameters);
		void retrieve(QString url, ViWebParameters parameters);

		QString stringResult();
		QJsonObject jsonResult();
		QByteArray& byteResult();

		QString url();

		QNetworkReply::NetworkError error();
		QString errorString();

		void clear();

	private:

		QNetworkRequest createGetRequest(QUrl url);
		QNetworkRequest createPostRequest(QUrl url);
		QNetworkRequest createJsonRequest(QUrl url);
		QNetworkRequest createRequest(QUrl url, QString contentType);

	private:

		QNetworkAccessManager mNetworkManager;
		QByteArray mData;
		QString mUrl;
		QNetworkReply::NetworkError mError;
		QString mErrorString;

};

#endif
