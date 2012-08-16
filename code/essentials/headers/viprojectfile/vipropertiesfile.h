#ifndef VIPROPERTIESFILE_H
#define VIPROPERTIESFILE_H

#include "viprojectfile.h"
#include "vimanager.h"

class ViPropertiesFile : public ViProjectFile
{

	public:

		ViPropertiesFile();
		ViVersion createdVersion();
		ViVersion editedVersion();
		QString projectName();
		void setProjectName(QString name);

	protected:

		QDomDocument toXml();
		void fromXml(QDomDocument document);

	private:

		ViVersion mCreatedVersion;
		ViVersion mEditedVersion;
		QString mProjectName;

};

#endif
