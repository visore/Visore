#include "vipropertiesinfo.h"
#include "vimanager.h"

ViPropertiesInfo::ViPropertiesInfo()
	: ViProjectInfo(), ViId()
{
}

void ViPropertiesInfo::initializeCurrent()
{
	setCreatedVersion(ViManager::version());
	setAccessedVersion(ViManager::version());
	setEditedVersion(ViManager::version());
	setCreatedDateTime(QDateTime::currentDateTime());
	setAccessedDateTime(QDateTime::currentDateTime());
	setEditedDateTime(QDateTime::currentDateTime());
	createId();
	setNull(false);
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
	setNull(false);
}

void ViPropertiesInfo::setAccessedVersion(ViVersion accessed)
{
	mAccessedVersion = accessed;
	setNull(false);
}

void ViPropertiesInfo::setEditedVersion(ViVersion edited)
{
	mEditedVersion  = edited;
	setNull(false);
}

void ViPropertiesInfo::setCreatedDateTime(QDateTime created)
{
	mCreatedDateTime = created;
	setNull(false);
}

void ViPropertiesInfo::setAccessedDateTime(QDateTime accessed)
{
	mAccessedDateTime = accessed;
	setNull(false);
}

void ViPropertiesInfo::setEditedDateTime(QDateTime edited)
{
	mEditedDateTime  = edited;
	setNull(false);
}

QString ViPropertiesInfo::toString()
{
	QString result = "Properties Info:";
	result += "\n\tId: " + id();
	result += "\n\tCreated: " + mCreatedDateTime.toString("yyyy-MM-dd hh:mm:ss") + " (version: " + mCreatedVersion.toString() + ")";
	result += "\n\tAccessed: " + mAccessedDateTime.toString("yyyy-MM-dd hh:mm:ss") + " (version: " + mAccessedVersion.toString() + ")";
	result += "\n\tEdited: " + mEditedDateTime.toString("yyyy-MM-dd hh:mm:ss") + " (version: " + mEditedVersion.toString() + ")";
	return result;
}
