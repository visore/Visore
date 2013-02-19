#include "vifont.h"

QString ViFont::styleSheet()
{
	QString result = "";
	
	result += "color: " + mColor.name() + ";";
	result += "font-size: " + QString::number(pointSize()) + ";";
	result += "font-family: " + family() + ";";

	return result;
}

void ViFont::setColor(QColor color)
{
	mColor = color;
}

QColor ViFont::color()
{
	return mColor;
}
