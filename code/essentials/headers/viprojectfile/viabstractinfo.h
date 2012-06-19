#ifndef VIABSTRACTINFO_H
#define VIABSTRACTINFO_H

#include "viprojectinfo.h"

class ViAbstractInfo
{

	public:

		virtual void fromInfo(ViProjectInfo &info) = 0;
		virtual ViProjectInfo& toInfo() = 0;

};

#endif
