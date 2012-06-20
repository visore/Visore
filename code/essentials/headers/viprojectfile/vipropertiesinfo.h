#ifndef VIPROPERTIESINFO_H
#define VIPROPERTIESINFO_H

#include "vimanager.h"
#include <QDateTime>

class ViPropertiesInfo
{

	public:

		static ViPropertiesInfo defaultProperties();

		ViVersion createdVersion();
		ViVersion accessedVersion();
		ViVersion editedVersion();
		QDateTime createdDateTime();
		QDateTime accessedDateTime();
		QDateTime editedDateTime();

		void setCreatedVersion(ViVersion created);
		void setAccessedVersion(ViVersion accessed);
		void setEditedVersion(ViVersion edited);
		void setCreatedDateTime(QDateTime created);
		void setAccessedDateTime(QDateTime accessed);
		void setEditedDateTime(QDateTime edited);

		QString toString();

	private:

		QDateTime mCreatedDateTime;
		QDateTime mAccessedDateTime;
		QDateTime mEditedDateTime;

		ViVersion mCreatedVersion;
		ViVersion mAccessedVersion;
		ViVersion mEditedVersion;

};

#endif
