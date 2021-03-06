#include "vimanager.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QRect>

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
	//Universal
	mVersion = ViVersion(QString(VERSION_MAJOR).toInt(), QString(VERSION_MINOR).toInt(), QString(VERSION_PATCH).toInt());
	mName = QString(NAME);
	mUrl = QUrl(QString(HOMEPAGE));
	mGitUrl = QUrl("https://github.com/visore/Visore");
	mEmail = QString(EMAIL);
	mLicense = QString(LICENSE);
	mCopyright = QString(COPYRIGHT);
	mDeveloper = QString(MAINTAINER);
	mShortDescription = QString(SHORT_DESCRIPTION);
	mLongDescription = QString(LONG_DESCRIPTION);
	mProjectExtension = "vip";
	mProjectName = "Visore Projects";

	mSettings = new QSettings(mName, mName);

	//Keys
	if(mSettings->value("keys/enmfp").isNull())
	{
		mSettings->setValue("keys/enmfp", "G1TZBE4IHJAYUSNCN");
	}
	if(mSettings->value("keys/acoustid").isNull())
	{
		mSettings->setValue("keys/acoustid", "wuknDrDT");
	}

	//Paths
	QString tempPath;
	if(mSettings->value("paths/temp").isNull())
	{
		tempPath = QDir::tempPath() + QDir::separator() + "visore" + QDir::separator();
		mSettings->setValue("paths/temp", tempPath);
	}
	else
	{
		tempPath = mSettings->value("paths/temp").toString();
	}
	QDir dir(tempPath);
	if(!dir.exists()) dir.mkpath(tempPath);

	QString path;
	if(mSettings->value("paths/tempcover").isNull())
	{
		path = tempPath + "covers" + QDir::separator();
		mSettings->setValue("paths/tempcover", path);
	}
	else
	{
		path = mSettings->value("paths/tempcover").toString();
	}
	dir = QDir(path);
	if(!dir.exists()) dir.mkpath(path);

	if(mSettings->value("paths/tempdata").isNull())
	{
		path = tempPath + "data" + QDir::separator();
		mSettings->setValue("paths/tempdata", path);
	}
	else
	{
		path = mSettings->value("paths/tempdata").toString();
	}
	dir = QDir(path);
	if(!dir.exists()) dir.mkpath(path);

	if(mSettings->value("paths/tempother").isNull())
	{
		path = tempPath + "other" + QDir::separator();
		mSettings->setValue("paths/tempother", path);
	}
	else
	{
		path = mSettings->value("paths/tempother").toString();
	}
	dir = QDir(path);
	if(!dir.exists()) dir.mkpath(path);

	if(mSettings->value("paths/project").isNull())
	{
		mSettings->setValue("paths/project", QDir::homePath() + QDir::separator() + "Visore Projects" + QDir::separator());
	}

	//Main window
	if(mSettings->value("interface/mainwindow/size").isNull())
	{
		mSettings->setValue("interface/mainwindow/size", QSize(800, 600));
	}
	if(mSettings->value("interface/mainwindow/fullscreen").isNull())
	{
		mSettings->setValue("interface/mainwindow/fullscreen", false);
	}
	if(mSettings->value("interface/mainwindow/maximized").isNull())
	{
		mSettings->setValue("interface/mainwindow/maximized", false);
	}
	if(mSettings->value("interface/mainwindow/position").isNull())
	{
		QRect geometry = QApplication::desktop()->screenGeometry();
		QSize window = mSettings->value("interface/mainwindow/size", QSize(800, 600)).toSize();
		mSettings->setValue("interface/mainwindow/position", QPoint((geometry.width() - window.width()) / 2, (geometry.height() - window.height()) / 2));
	}

	mSettings->sync();
}

ViManager::~ViManager()
{
	delete mSettings;
}

void ViManager::setValue(QString key, QVariant value)
{
	instance()->mSettings->setValue(key, value);
	instance()->mSettings->sync();
}

QVariant ViManager::value(QString key)
{
	return instance()->mSettings->value(key);
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

QUrl ViManager::gitUrl()
{
	return instance()->mGitUrl;
}

QString ViManager::email()
{
	return instance()->mEmail;
}

QString ViManager::license()
{
	return instance()->mLicense;
}

QString ViManager::developer()
{
	return instance()->mDeveloper;
}

QString ViManager::copyright()
{
	return instance()->mCopyright;
}

QString ViManager::shortDescription()
{
	return instance()->mShortDescription;
}

QString ViManager::longDescription()
{
	return instance()->mLongDescription;
}

QString ViManager::projectExtension()
{
	return instance()->mProjectExtension;
}

QString ViManager::projectName()
{
	return instance()->mProjectName;
}

QString ViManager::projectFilter()
{
	return projectName() + " (*." + projectExtension() + ")";
}

QString ViManager::enmfpKey()
{
	return ViManager::value("keys/enmfp").toString();
}

QString ViManager::acoustidKey()
{
	return ViManager::value("keys/acoustid").toString();
}

QString ViManager::tempPath()
{
	return ViManager::value("paths/temp").toString();
}

void ViManager::setTempPath(QString path)
{
	ViManager::setValue("paths/temp", path);
}

QString ViManager::tempCoverPath()
{
	return ViManager::value("paths/tempcover").toString();
}

void ViManager::setTempCoverPath(QString path)
{
	ViManager::setValue("paths/tempcover", path);
}

QString ViManager::tempDataPath()
{
	return ViManager::value("paths/tempdata").toString();
}

void ViManager::setTempDataPath(QString path)
{
	ViManager::setValue("paths/tempdata", path);
}

QString ViManager::tempOtherPath()
{
	return ViManager::value("paths/tempother").toString();
}

void ViManager::setTempOtherPath(QString path)
{
	ViManager::setValue("paths/tempother", path);
}

QString ViManager::projectPath()
{
	return ViManager::value("paths/project").toString();
}

void ViManager::setProjectPath(QString path)
{
	ViManager::setValue("paths/project", path);
}

QSize ViManager::windowSize()
{
	return ViManager::value("interface/mainwindow/size").toSize();
}

bool ViManager::isWindowFullScreen()
{
	return ViManager::value("interface/mainwindow/fullscreen").toBool();
}

bool ViManager::isWindowMaximized()
{
	return ViManager::value("interface/mainwindow/maximized").toBool();
}

QPoint ViManager::windowPosition()
{
	return ViManager::value("interface/mainwindow/position").toPoint();
}

void ViManager::setWindowSize(QSize size)
{
	ViManager::setValue("interface/mainwindow/size", size);
}

void ViManager::setWindowFullScreen(bool fullScreen)
{
	ViManager::setValue("interface/mainwindow/fullscreen", fullScreen);
}

void ViManager::setWindowMaximized(bool maximized)
{
	ViManager::setValue("interface/mainwindow/maximized", maximized);
}

void ViManager::setWindowPosition(QPoint position)
{
	ViManager::setValue("interface/mainwindow/position", position);
}
