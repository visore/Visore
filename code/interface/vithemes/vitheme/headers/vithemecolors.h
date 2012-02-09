#ifndef VITHEMECOLORS_H
#define VITHEMECOLORS_H

#define MAXIMUM_COLORS 25

#include <QColor>
#include "vierror.h"

class ViThemeColors : public ViError
{
	public:
		ViThemeColors();
		QColor color(int index);

	protected:
		QList<QColor> mColors;
};

#endif
