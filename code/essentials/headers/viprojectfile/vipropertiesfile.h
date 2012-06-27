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

	protected:

		QDomDocument toXml();
		void fromXml(QDomDocument document);

	private:

		ViVersion mCreatedVersion;
		ViVersion mEditedVersion;

};

#endif
