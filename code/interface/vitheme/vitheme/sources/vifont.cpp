#include "vifont.h"

QString ViFont::styleSheet()
{
	QString result = "";
	
	result += "color: " + mColor.name() + ";";
    result += "font-size: " + QString::number(pointSize()) + "px;";
	result += "font-family: " + family() + ";";
    if(bold())
    {
        result += "font-weight: bold;";
    }

	return result;
}

void ViFont::setColor(QColor color)
{
	mColor = color;
}

QColor ViFont::color() const
{
	return mColor;
}
