#include "viprojectinfo.h"

ViProjectInfo::ViProjectInfo()
{
	setNull(true);
}

bool ViProjectInfo::isNull()
{
	return mIsNull;
}

void ViProjectInfo::setNull(bool null)
{
	mIsNull = null;
}
