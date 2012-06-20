#include "vipropertiesinfo.h"
#include "vimanager.h"

ViPropertiesInfo ViPropertiesInfo::defaultProperties()
{
	ViPropertiesInfo result;
	result.setCreatedVersion(ViManager::version());
	result.setAccessedVersion(ViManager::version());
	result.setEditedVersion(ViManager::version());
	result.setCreatedDateTime(QDateTime::currentDateTime());
	result.setAccessedDateTime(QDateTime::currentDateTime());
	result.setEditedDateTime(QDateTime::currentDateTime());
	return result;
}

ViVersion ViPropertiesInfo::createdVersion()
{
	return mCreatedVersion;
}

ViVersion ViPropertiesInfo::accessedVersion()
{
	return mAccessedVersion;
}

ViVersion ViPropertiesInfo::editedVersion()
{
	return mEditedVersion;
}

QDateTime ViPropertiesInfo::createdDateTime()
{
	return mCreatedDateTime;
}

QDateTime ViPropertiesInfo::accessedDateTime()
{
	return mAccessedDateTime;
}

QDateTime ViPropertiesInfo::editedDateTime()
{
	return mEditedDateTime;
}

void ViPropertiesInfo::setCreatedVersion(ViVersion created)
{
	mCreatedVersion = created;
}

void ViPropertiesInfo::setAccessedVersion(ViVersion accessed)
{
	mAccessedVersion = accessed;
}

void ViPropertiesInfo::setEditedVersion(ViVersion edited)
{
	mEditedVersion  = edited;
}

void ViPropertiesInfo::setCreatedDateTime(QDateTime created)
{
	mCreatedDateTime = created;
}

void ViPropertiesInfo::setAccessedDateTime(QDateTime accessed)
{
	mAccessedDateTime = accessed;
}

void ViPropertiesInfo::setEditedDateTime(QDateTime edited)
{
	mEditedDateTime  = edited;
}

QString ViPropertiesInfo::toString()
{
	QString result = "";
	result += "Created: " + mCreatedDateTime.toString("yyyy-MM-dd hh:mm:ss") + " (version: " + mCreatedVersion.toString() + ")\n";
	result += "Accessed: " + mAccessedDateTime.toString("yyyy-MM-dd hh:mm:ss") + " (version: " + mAccessedVersion.toString() + ")\n";
	result += "Edited: " + mEditedDateTime.toString("yyyy-MM-dd hh:mm:ss") + " (version: " + mEditedVersion.toString() + ")";
	return result;
}
