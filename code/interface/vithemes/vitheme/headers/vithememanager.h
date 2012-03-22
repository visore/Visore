#ifndef VITHEMEMANAGER_H
#define VITHEMEMANAGER_H

#include "vitheme.h"
#include "vithemeimage.h"
#include "vilibrary.h"
#include "vilibrarydetector.h"
#include <QIcon>

class ViThemeManager
{
	public:
		enum Type
		{
			Icon = 0,
			Background = 1
		};
		static QList<QString> themes(); 
		static void setTheme(QString name);
		~ViThemeManager(); //Must be public for QSharedPointer

		static QColor color(int index);
		static ViThemeImage image(QString name, ViThemeImage::State state, ViThemeManager::Type type);
		
	protected:
		ViThemeManager();
		static ViThemeManager* instance();
		static QString stateString(ViThemeImage::State state);

	protected:
		QList<ViTheme*> mThemes;
		ViTheme* mCurrentTheme;
		QList<ViLibrary<ViTheme>*> mLibraries;
		static QSharedPointer<ViThemeManager> mInstance;
};

#endif
