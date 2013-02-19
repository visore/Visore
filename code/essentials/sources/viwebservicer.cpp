#include <viwebservicer.h>
#include <QJsonDocument>
#include <QUrlQuery>
#include <vimanager.h>

ViWebParameter::ViWebParameter()
{
	mName = "";
	mValue = "";
}

ViWebParameter::ViWebParameter(QString name, QString value)
{
	mName = name;
	mValue = value;
}

ViWebParameter::ViWebParameter(const ViWebParameter &other)
{
	mName = other.mName;
	mValue = other.mValue;
}

void ViWebParameter::setName(QString name)
{
	mName = name;
}

void ViWebParameter::setValue(QString value)
{
	mValue = value;
}

QString ViWebParameter::name()
{
	return mName;
}

QString ViWebParameter::value()
{
	return mValue;
}

ViWebParameters::ViWebParameters()
	: QList<ViWebParameter>()
{
}

void ViWebParameters::append(QString name, QString value)
{
	QList<ViWebParameter>::append(ViWebParameter(name, value));
}

ViWebServicer::ViWebServicer()
{
	QObject::connect(&mNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(processReply(QNetworkReply*)));
}

void ViWebServicer::setProxy(QNetworkProxy::ProxyType type, QString host, quint16 port, QString username, QString password)
{
	QNetworkProxy proxy(type, host, port, username, password);
	mNetworkManager.setProxy(proxy);
}

void ViWebServicer::retrieve(QUrl url)
{
	clear();
	mNetworkManager.get(createGetRequest(url));
}

void ViWebServicer::retrieve(QString url)
{
	retrieve(QUrl(url));
}

void ViWebServicer::retrieve(QUrl url, QJsonObject jsonObject)
{
	clear();
	mNetworkManager.post(createJsonRequest(url), QJsonDocument(jsonObject).toJson());
}

void ViWebServicer::retrieve(QString url, QJsonObject jsonObject)
{
	retrieve(QUrl(url), jsonObject);
}

void ViWebServicer::retrieve(QUrl url, ViWebParameters parameters)
{
	clear();
	QUrlQuery data;
	for(int i = 0; i < parameters.size(); ++i)
	{
		data.addQueryItem(parameters[i].name(), parameters[i].value());
	}
	mNetworkManager.post(createPostRequest(url), data.query().toLatin1());
}

void ViWebServicer::retrieve(QString url, ViWebParameters parameters)
{
	retrieve(QUrl(url), parameters);
}

void ViWebServicer::processReply(QNetworkReply *reply)
{
	mError = reply->error();
	if(mError == QNetworkReply::NoError)
	{
		QString url = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl().toString();
		if(url == "")
		{
			mData = reply->readAll();
			mUrl = reply->url().toString();
			emit finished(true);
		}
		else
		{
			if(!url.contains("://"))
			{
				url = reply->url().resolved(QUrl(url)).toString();
			}
			retrieve(url);
		}
	}
	else
	{
		emit finished(false);
	}
}

QString ViWebServicer::stringResult()
{
	return QString(mData);
}

QJsonObject ViWebServicer::jsonResult()
{
	return QJsonDocument::fromJson(mData).object();
}

QByteArray& ViWebServicer::byteResult()
{
	return mData;
}

QString ViWebServicer::url()
{
	return mUrl;
}

QNetworkReply::NetworkError ViWebServicer::error()
{
	return mError;
}

QNetworkRequest ViWebServicer::createGetRequest(QUrl url)
{
	return createRequest(url, "text/plain");
}

QNetworkRequest ViWebServicer::createPostRequest(QUrl url)
{
	return createRequest(url, "application/x-www-form-urlencoded");
}

QNetworkRequest ViWebServicer::createJsonRequest(QUrl url)
{
	return createRequest(url, "application/octet-stream");
}

QNetworkRequest ViWebServicer::createRequest(QUrl url, QString contentType)
{
	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, contentType);
	request.setRawHeader("User-Agent", QString(ViManager::name() + "/" + ViManager::version().toString()).toLatin1());
	return request;
}

void ViWebServicer::clear()
{
	mData.clear();
	mUrl = "";
	mError = QNetworkReply::NoError;
}
