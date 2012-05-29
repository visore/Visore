#include "viorangetheme.h"

ViOrangeTheme::ViOrangeTheme()
	: ViTheme()
{
	mColors = new ViOrangeThemeColors();
}

ViTheme* ViOrangeTheme::instance()
{
	if(mInstance.isNull())
	{
		mInstance = QSharedPointer<ViOrangeTheme>(new ViOrangeTheme()).dynamicCast<ViTheme>();
	}
	return mInstance.data();
}

#ifdef __cplusplus
extern "C"
{
#endif

ViTheme* createTheme()
{
   return ViOrangeTheme::instance();
}

#ifdef __cplusplus
}
#endif
