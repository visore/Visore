#include "vithemecolors.h"

ViThemeColors::ViThemeColors()
{
	for(int i = 0; i < MAXIMUM_COLORS; ++i)
	{
		mColors.append(Qt::black);
	}
}

QColor ViThemeColors::color(int index)
{
	if(index >= mColors.size())
	{
		//setErrorParameters("ViThemeColor - Selected Color", "The color you requested is not available", ViErrorInfo::NonFatal);
	}
	return mColors[index];
}
