#ifndef VIFILER_H
#define VIFILER_H

#include "vipropertieselement.h"
#include <QFile>

class ViFiler
{

	public:

		ViFiler(QString filePath = "");
		~ViFiler();
		void setFilePath(QString filePath);
		QString filePath();

		ViPropertiesElement* properties();

	protected:

		bool open();
		bool save();
		void close();

	protected:

		QFile mFile;
		QDomDocument mDocument;
		ViPropertiesElement *mProperties;

};

#endif
