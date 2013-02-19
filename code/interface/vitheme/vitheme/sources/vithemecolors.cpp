#include "vithemecolors.h"

ViThemeColors::ViThemeColors()
{
}

QColor ViThemeColors::color(ViThemeColors::Type type)
{
	return mColors[type];
}

void ViThemeColors::addColor(QColor color, ViThemeColors::Type type)
{
	mColors[type] = color;
}
