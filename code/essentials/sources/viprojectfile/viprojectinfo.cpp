#include "viprojectinfo.h"

ViProjectInfo::ViProjectInfo(QString name)
	: ViInfoMap()
{
	mName = name;
}

void ViProjectInfo::setName(QString name)
{
	mName = name;
}

QString ViProjectInfo::name()
{
	return mName;
}
