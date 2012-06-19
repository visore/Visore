#ifndef VIPROPERTIESELEMENT_H
#define VIPROPERTIESELEMENT_H

#include "vifilerelement.h"
#include "vimanager.h"
#include <QDateTime>

class ViPropertiesElement : public ViFilerElement
{

	public:

		ViPropertiesElement(QDomDocument *document);

		ViVersion createdVersion();
		ViVersion editedVersion();
		QDateTime createdDateTime();
		QDateTime editedDateTime();

		void setCreatedVersion(ViVersion created);
		void setEditedVersion(ViVersion edited);
		void setCreatedDateTime(QDateTime created);
		void setEditedDateTime(QDateTime edited);

		QDomElement toDom();

	protected:

		bool fromDom();

	private:

		QString mDescription;
		ViVersion mCreatedVersion;
		ViVersion mEditedVersion;
		QDateTime mCreatedDateTime;
		QDateTime mEditedDateTime;

};

#endif
