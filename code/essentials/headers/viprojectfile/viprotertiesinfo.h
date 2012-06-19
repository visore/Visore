#ifndef VIPROPERTIESINFO_H
#define VIPROPERTIESINFO_H

#include "viabstractinfo.h"
#include "vimanager.h"
#include <QDateTime>

class ViPropertiesInfo : public ViAbstractInfo
{

	public:

		void fromInfo(ViProjectInfo &info);
		ViProjectInfo& toInfo();

	private:

		QDateTime mCreatedDateTime;
		QDateTime mAccessedDateTime;
		QDateTime mEditedDateTime;

		ViVersion mCreatedVersion;
		ViVersion mAccessedVersion;
		ViVersion mEditedVersion;

};

#endif
