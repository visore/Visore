#ifndef VITHEMECOLORS_H
#define VITHEMECOLORS_H

#include <QColor>
#include <QMap>

class ViThemeColors
{

	public:

		enum Type
		{
			MainColor1,
			MainColor2,
			MainColor3,
			MainColor4,
			MainColor5,
			MainColor6,
			MainColor7,

			ButtonTextColor1,
			ButtonTextColor2,

			ButtonNormalColor1,
			ButtonNormalColor2,
			ButtonSelectedColor1,
			ButtonSelectedColor2,
			ButtonHoveredColor1,
			ButtonHoveredColor2,
			ButtonDisbaledColor1,
			ButtonDisbaledColor2,

			ButtonGlowColor1,

			TextColor1,
			TextColor2,

			BorderColor1,
			BorderColor2,
			BorderColor3
		};

	public:

		ViThemeColors();
		QColor color(ViThemeColors::Type type);

	protected:

		void addColor(QColor color, ViThemeColors::Type type);

	protected:

		QMap<ViThemeColors::Type, QColor> mColors;

};

#endif
