#ifndef VIPROJECTPROPERTIES_H
#define VIPROJECTPROPERTIES_H

#include "viprojectinfo.h"
#include "vimanager.h"

class ViProjectProperties : public ViProjectInfo
{

	public:

		ViProjectProperties();
		ViVersion createdVersion();
		ViVersion editedVersion();
		QString projectName();
		void setProjectName(QString name);

	protected:

		ViElement toXml();
		bool fromXml(ViElement &document);

	private:

		ViVersion mCreatedVersion;
		ViVersion mEditedVersion;
		QString mProjectName;

};

#endif
