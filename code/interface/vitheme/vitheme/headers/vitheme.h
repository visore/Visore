#ifndef VITHEME_H
#define VITHEME_H

#include <QString>
#include <vithemecolors.h>
#include <vithemefonts.h>
#include <visingleton.h>

class ViTheme
{

	public:

		virtual ~ViTheme();

		QString globalStyleSheet();

		QString name();
		ViThemeColors* colors();
		ViThemeFonts* fonts();

	protected:

		ViTheme();

	protected:

		QString mGlobalStyleSheet;
		QString mName;
		ViThemeColors *mColors;
		ViThemeFonts *mFonts;

};

typedef ViPointer<ViTheme> ViThemePointer;

#endif
