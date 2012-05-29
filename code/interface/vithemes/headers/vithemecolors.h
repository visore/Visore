#ifndef VITHEMECOLORS_H
#define VITHEMECOLORS_H

#define MAXIMUM_COLORS 25

#include <QColor>

class ViThemeColors
{
	public:
		ViThemeColors();
		QColor color(int index);

	protected:
		QList<QColor> mColors;
};

#endif
