#ifndef VITHEMEMANAGER_H
#define VITHEMEMANAGER_H

#include "vitheme.h"
#include "vilibrary.h"
#include "vilibrarydetector.h"

class ViThemeManager
{
	public:
		static QList<QString> themes(); 
		static void setTheme(QString name);
		~ViThemeManager(); //Must be public for QSharedPointer

		static QColor color(int index);
		
	protected:
		ViThemeManager();
		static ViThemeManager* instance();

	protected:
		QList<ViTheme*> mThemes;
		ViTheme* mCurrentTheme;
		QList<ViLibrary<ViTheme>*> mLibraries;
		static QSharedPointer<ViThemeManager> mInstance;
};

#endif
