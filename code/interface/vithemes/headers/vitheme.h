#ifndef VITHEME_H
#define VITHEME_H

#include <QString>
#include <QSharedPointer>
#include "vithemecolors.h"

class ViTheme
{
	public:
		static ViTheme* instance();
		~ViTheme();
		QString name();
		ViThemeColors* colors();

	protected:
		ViTheme();

	protected:
		static QSharedPointer<ViTheme> mInstance;
		QString mName;
		ViThemeColors *mColors;
};

#endif
