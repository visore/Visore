#include "vimanager.h"

ViVersion::ViVersion(quint8 major, quint8 minor, quint8 patch)
{
	mMajor = major;
	mMinor = minor;
	mPatch = patch;
}

quint8 ViVersion::major()
{
	return mMajor;
}

quint8 ViVersion::minor()
{
	return mMinor;
}

quint8 ViVersion::patch()
{
	return mPatch;
}

QString ViVersion::toString()
{
	return QString::number(mMajor) + "." + QString::number(mMinor) + "." + QString::number(mPatch);
}

ViManager::ViManager()
{
	mVersion = ViVersion(QString(VERSION_MAJOR).toInt(), QString(VERSION_MINOR).toInt(), QString(VERSION_PATCH).toInt());
	mName = QString(NAME);
	mUrl = QUrl(QString(HOMEPAGE).replace("http://", ""));
}

ViVersion ViManager::version()
{
	return instance()->mVersion;
}

QString ViManager::name()
{
	return instance()->mName;
}

QUrl ViManager::url()
{
	return instance()->mUrl;
}
