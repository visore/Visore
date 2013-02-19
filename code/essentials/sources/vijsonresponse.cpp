#include <vijsonresponse.h>
#include <QJsonDocument>
#include <QJsonObject>

ViJsonResponse::ViJsonResponse()
{
}

ViJsonResponse::ViJsonResponse(QByteArray json)
{
	analyze(json);
}

ViJsonResponse::~ViJsonResponse()
{
}

bool ViJsonResponse::analyze(QByteArray json)
{
	QJsonDocument document = QJsonDocument::fromJson(json);
	mMap = document.object().toVariantMap();
	return analyzeResponse();
}

QVariantMap& ViJsonResponse::map()
{
	return mMap;
}
