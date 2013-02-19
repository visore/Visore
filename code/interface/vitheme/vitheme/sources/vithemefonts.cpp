#include "vithemefonts.h"
#include <QFontDatabase>
#include <vilogger.h>

ViThemeFonts::ViThemeFonts()
{

}

ViFont ViThemeFonts::font(ViThemeFonts::Type type)
{
	return mFonts[type];
}

void ViThemeFonts::addFont(QString fileName, QString fontName, ViThemeFonts::Type type)
{
	if(QFontDatabase::addApplicationFont(":/fonts/" + fileName + ".ttf") == -1)
	{
		LOG("The font \"" + fontName + "\" could not be added. If you are using Linux, make sure fontconfig is installed.", QtCriticalMsg);
	}
	else 
	{
		ViFont font;
		font.setFamily(fontName);
		mFonts[type] = font;
	}
}
