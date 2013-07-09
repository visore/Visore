#ifndef VITHEMEMANAGER_H
#define VITHEMEMANAGER_H

#include "vitheme.h"
#include "vithemeimage.h"
#include "vithemeicon.h"
#include "vithemeanimation.h"
#include "vithemefonts.h"
#include <visingleton.h>
#include <QLibrary>

class ViThemeManager : public ViSingleton<ViThemeManager>
{

	friend class ViSingleton<ViThemeManager>;

	public:

		enum Type
		{
			Icon = 0,
			Background = 1,
			Animation = 2
		};

		~ViThemeManager(); //Must be public for QSharedPointer

		static QString globalStyleSheet();

		static QColor color(ViThemeColors::Type type);
		static ViFont font(ViThemeFonts::Type type);

		static ViThemeImage image(QString name);
		static ViThemeIcon icon(QString name);
		static ViThemeAnimation animation(QString name);
		
	protected:

		ViThemeManager();
		
	protected:

		QLibrary mLibrary;
		ViThemePointer mCurrentTheme;

};

#endif
