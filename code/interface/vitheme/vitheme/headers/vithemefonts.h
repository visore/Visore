#ifndef VITHEMEFONTS_H
#define VITHEMEFONTS_H

#include <vifont.h>
#include <QMap>

class ViThemeFonts
{

	public:

		enum Type
		{
			MainFont
		};

	public:

		ViThemeFonts();
		ViFont font(ViThemeFonts::Type type);

	protected:

		void addFont(QString fileName, QString fontName, ViThemeFonts::Type type);

	protected:

		QMap<ViThemeFonts::Type, ViFont> mFonts;

};

#endif
