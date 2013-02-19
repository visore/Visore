#ifndef VIJSONRESPONSE_H
#define VIJSONRESPONSE_H

#include <QByteArray>
#include <QVariantMap>

class ViJsonResponse
{

	public:

		ViJsonResponse();
		ViJsonResponse(QByteArray json);
		virtual ~ViJsonResponse();

		bool analyze(QByteArray json);

	protected:

		QVariantMap& map();

		virtual bool analyzeResponse() = 0;

	private:

		QVariantMap mMap;

};

#endif
