#ifndef VITHEMEMANAGER_H
#define VITHEMEMANAGER_H

#include "vitheme.h"
#include "vilibrary.h"
#include "vilibrarydetector.h"
#include <QIcon>

class ViThemeManager
{
	public:
		enum ViIconType
		{
			Normal = 0,
			Hover = 1,
			Selected = 2
		};

		static QList<QString> themes(); 
		static void setTheme(QString name);
		~ViThemeManager(); //Must be public for QSharedPointer

		static QColor color(int index);
		static QIcon icon(QString name, ViThemeManager::ViIconType type = ViThemeManager::Normal);
		static QString iconPath(QString name, ViThemeManager::ViIconType type = ViThemeManager::Normal);
		static QImage image(QString name, ViThemeManager::ViIconType type = ViThemeManager::Normal);
		static QImage background(QString name);
		
	protected:
		ViThemeManager();
		static ViThemeManager* instance();
		static QString typeString(ViThemeManager::ViIconType type);

	protected:
		QList<ViTheme*> mThemes;
		ViTheme* mCurrentTheme;
		QList<ViLibrary<ViTheme>*> mLibraries;
		static QSharedPointer<ViThemeManager> mInstance;
};

#endif
