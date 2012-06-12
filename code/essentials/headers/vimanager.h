#ifndef VIMANAGER_H
#define VIMANAGER_H

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

		static ViVersion version();
		static QString name();
		static QUrl url();

	protected:

		friend class ViSingleton<ViManager>;
		ViManager();

	private:

		ViVersion mVersion;
		QString mName;
		QUrl mUrl;

};

#endif
