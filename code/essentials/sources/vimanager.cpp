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
	mUrl = QUrl(QString(HOMEPAGE).replace("http://", ""));
	mProjectExtension = "vip";

	mSettings = new QSettings(mName, mName);

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

QString ViManager::projectExtension()
{
	return instance()->mProjectExtension;
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
