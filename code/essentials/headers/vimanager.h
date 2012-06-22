#ifndef VIMANAGER_H
#define VIMANAGER_H

#include <QSettings>
#include <QSize>
#include <QDir>
#include <QUrl>
#include <QtGlobal>
#include "visingleton.h"

class ViVersion
{

	public:

		ViVersion(quint8 major = 0, quint8 minor = 0, quint8 patch = 0);
		quint8 major();
		quint8 minor();
		quint8 patch();
		QString toString();

	private:

		quint8 mMajor;
		quint8 mMinor;
		quint8 mPatch;

};

class ViManager : public ViSingleton<ViManager>
{

	public:

		~ViManager();

		//Universal
		static ViVersion version();
		static QString name();
		static QUrl url();
		static QString projectExtension();

		//Paths
		static QString tempPath();
		static void setTempPath(QString path);

		//Main window
		static QSize windowSize();
		static bool isWindowFullScreen();
		static bool isWindowMaximized();
		static QPoint windowPosition();

		static void setWindowSize(QSize size);
		static void setWindowFullScreen(bool fullScreen);
		static void setWindowMaximized(bool maximized);
		static void setWindowPosition(QPoint position);

	protected:

		friend class ViSingleton<ViManager>;
		ViManager();
		static void setValue(QString key, QVariant value);
		static QVariant value(QString key);

	private:

		QSettings *mSettings;
		ViVersion mVersion;
		QString mName;
		QUrl mUrl;
		QString mProjectExtension;
		QString mTempPath;

};

#endif
