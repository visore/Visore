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

	friend class ViSingleton<ViManager>;

	public:

		~ViManager();

		//Universal
		static ViVersion version();
		static QString name();
		static QUrl url();
		static QString projectExtension();
		static QString projectName();
		static QString projectFilter();

		//Keys
		static QString enmfpKey();
		static QString acoustidKey();

		//Paths
		static QString tempPath();
		static void setTempPath(QString path);
		static QString tempCoverPath();
		static void setTempCoverPath(QString path);
		static QString tempDataPath();
		static void setTempDataPath(QString path);

		static QString projectPath();
		static void setProjectPath(QString path);

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

		ViManager();
		static void setValue(QString key, QVariant value);
		static QVariant value(QString key);

	private:

		QSettings *mSettings;
		ViVersion mVersion;
		QString mName;
		QUrl mUrl;
		QString mProjectExtension;
		QString mProjectName;

		QString mTempPath;
		QString mTempCoverPath;
		QString mTempDataPath;
		QString mProjectPath;
		QString mThemesPath;

};

#endif
