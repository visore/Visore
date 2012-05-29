#ifndef VITHEMEMANAGER_H
#define VITHEMEMANAGER_H

#include "vitheme.h"
#include "vithemeimage.h"
#include <QIcon>

class ViThemeManager
{
	public:
		enum Type
		{
			Icon = 0,
			Background = 1,
			Animation = 2
		};
		~ViThemeManager(); //Must be public for QSharedPointer

		static QColor color(int index);
		static ViThemeImage image(QString name, ViThemeImage::State state, ViThemeManager::Type type);
		
	protected:
		ViThemeManager();
		static ViThemeManager* instance();
		static QString stateString(ViThemeImage::State state);

	protected:
		ViTheme* mCurrentTheme;
		static QSharedPointer<ViThemeManager> mInstance;
};

#endif
